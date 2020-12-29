#include "IMU.h"
#include "attachInterruptEx.h"

IMU imu;

void IMU::self_test() // Should return percent deviation from factory trim values, +/- 14 or less deviation is a pass
{
	uint8_t rawData[4];
	uint8_t selfTest[6];
	float factoryTrim[6];

	// Configure the accelerometer for self-test
	writeRegister8(MPU6050_ADDRESS, ACCEL_CONFIG, 0xF0); // Enable self test on all three axes and set accelerometer range to +/- 8 g
	writeRegister8(MPU6050_ADDRESS, GYRO_CONFIG, 0xE0); // Enable self test on all three axes and set gyro range to +/- 250 degrees/s
	delay(250);  // Delay a while to let the device execute the self-test
	rawData[0] = readRegister8(MPU6050_ADDRESS, SELF_TEST_X); // X-axis self-test results
	rawData[1] = readRegister8(MPU6050_ADDRESS, SELF_TEST_Y); // Y-axis self-test results
	rawData[2] = readRegister8(MPU6050_ADDRESS, SELF_TEST_Z); // Z-axis self-test results
	rawData[3] = readRegister8(MPU6050_ADDRESS, SELF_TEST_A); // Mixed-axis self-test results

	// Extract the acceleration test results first
	selfTest[0] = (rawData[0] >> 3) | (rawData[3] & 0x30) >> 4; // XA_TEST result is a five-bit unsigned integer
	selfTest[1] = (rawData[1] >> 3) | (rawData[3] & 0x0C) >> 2; // YA_TEST result is a five-bit unsigned integer
	selfTest[2] = (rawData[2] >> 3) | (rawData[3] & 0x03); // ZA_TEST result is a five-bit unsigned integer
												
	// Extract the gyration test results first
	selfTest[3] = rawData[0] & 0x1F; // XG_TEST result is a five-bit unsigned integer
	selfTest[4] = rawData[1] & 0x1F; // YG_TEST result is a five-bit unsigned integer
	selfTest[5] = rawData[2] & 0x1F; // ZG_TEST result is a five-bit unsigned integer   
										
	// Process results to allow final comparison with factory set values
	factoryTrim[0] = (4096.0*0.34)*(powf((0.92 / 0.34), (((float)selfTest[0] - 1.0) / 30.0))); // FT[Xa] factory trim calculation
	factoryTrim[1] = (4096.0*0.34)*(powf((0.92 / 0.34), (((float)selfTest[1] - 1.0) / 30.0))); // FT[Ya] factory trim calculation
	factoryTrim[2] = (4096.0*0.34)*(powf((0.92 / 0.34), (((float)selfTest[2] - 1.0) / 30.0))); // FT[Za] factory trim calculation
	factoryTrim[3] = (25.0*131.0)*(powf(1.046, ((float)selfTest[3] - 1.0)));             // FT[Xg] factory trim calculation
	factoryTrim[4] = (-25.0*131.0)*(powf(1.046, ((float)selfTest[4] - 1.0)));             // FT[Yg] factory trim calculation
	factoryTrim[5] = (25.0*131.0)*(powf(1.046, ((float)selfTest[5] - 1.0)));             // FT[Zg] factory trim calculation

	// Report results as a ratio of (STR - FT)/FT; the change from Factory Trim of the Self-Test Response
	// To get to percent, must multiply by 100 and subtract result from 100

	for (int i = 0; i < 6; i++) 
	{
		selfTestResult[i] = 100.0 + 100.0*((float)selfTest[i] - factoryTrim[i]) / factoryTrim[i]; // Report percent differences
	}

	/*
	Serial.print("x-axis self test: acceleration trim within : "); Serial.print(selfTestResult[0],1); Serial.println("% of factory value");
	Serial.print("y-axis self test: acceleration trim within : "); Serial.print(selfTestResult[1],1); Serial.println("% of factory value");
	Serial.print("z-axis self test: acceleration trim within : "); Serial.print(selfTestResult[2],1); Serial.println("% of factory value");
	Serial.print("x-axis self test: gyration trim within : "); Serial.print(selfTestResult[3],1); Serial.println("% of factory value");
	Serial.print("y-axis self test: gyration trim within : "); Serial.print(selfTestResult[4],1); Serial.println("% of factory value");
	Serial.print("z-axis self test: gyration trim within : "); Serial.print(selfTestResult[5],1); Serial.println("% of factory value");
	*/
}

void IMU::calibrate()
{
	uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
	uint16_t ii, packet_count, fifo_count;
	int32_t gyro_bias[3] = { 0, 0, 0 }, accel_bias[3] = { 0, 0, 0 };

	// reset device, reset all registers, clear gyro and accelerometer bias registers
	writeRegister8(MPU6050_ADDRESS, PWR_MGMT_1, 0x80); // Write a one to bit 7 reset bit; toggle reset device
	delay(100);

	// get stable time source
	// Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001
	writeRegister8(MPU6050_ADDRESS, PWR_MGMT_1, 0x01);
	writeRegister8(MPU6050_ADDRESS, PWR_MGMT_2, 0x00);
	delay(200);

	// Configure device for bias calculation
	writeRegister8(MPU6050_ADDRESS, INT_ENABLE, 0x00);   // Disable all interrupts
	writeRegister8(MPU6050_ADDRESS, FIFO_EN, 0x00);      // Disable FIFO
	writeRegister8(MPU6050_ADDRESS, PWR_MGMT_1, 0x00);   // Turn on internal clock source
	writeRegister8(MPU6050_ADDRESS, I2C_MST_CTRL, 0x00); // Disable I2C master
	writeRegister8(MPU6050_ADDRESS, USER_CTRL, 0x00);    // Disable FIFO and I2C master modes
	writeRegister8(MPU6050_ADDRESS, USER_CTRL, 0x0C);    // Reset FIFO and DMP
	delay(15);

	// Configure MPU6050 gyro and accelerometer for bias calculation
	writeRegister8(MPU6050_ADDRESS, DLPF_CONFIG, 0x01);      // Set low-pass filter to 188 Hz
	writeRegister8(MPU6050_ADDRESS, SMPLRT_DIV, 0x00);  // Set sample rate to 1 kHz
	writeRegister8(MPU6050_ADDRESS, GYRO_CONFIG, 0x00);  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
	writeRegister8(MPU6050_ADDRESS, ACCEL_CONFIG, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity

	uint16_t gyrosensitivity = 131;   // = 131 LSB/degrees/sec
	uint16_t accelsensitivity = 16384;  // = 16384 LSB/g

	// Configure FIFO to capture accelerometer and gyro data for bias calculation
	writeRegister8(MPU6050_ADDRESS, USER_CTRL, 0x40);   // Enable FIFO  
	writeRegister8(MPU6050_ADDRESS, FIFO_EN, 0x78);     // Enable gyro and accelerometer sensors for FIFO  (max size 1024 bytes in MPU-6050)
	delay(80); // accumulate 80 samples in 80 milliseconds = 960 bytes

	// At end of sample accumulation, turn off FIFO sensor read
	writeRegister8(MPU6050_ADDRESS, FIFO_EN, 0x00);        // Disable gyro and accelerometer sensors for FIFO
	readBytes(MPU6050_ADDRESS, FIFO_COUNTH, 2, &data[0]); // read FIFO sample count
	fifo_count = ((uint16_t)data[0] << 8) | data[1];
	packet_count = fifo_count / 12;// How many sets of full gyro and accelerometer data for averaging

	for (ii = 0; ii < packet_count; ii++) 
	{
		readBytes(MPU6050_ADDRESS, FIFO_R_W, 12, &data[0]); // read data for averaging

		accel_bias[0] += (int32_t)(((int16_t)data[0] << 8) | data[1]);
		accel_bias[1] += (int32_t)(((int16_t)data[2] << 8) | data[3]);
		accel_bias[2] += (int32_t)(((int16_t)data[4] << 8) | data[5]);
		gyro_bias[0] += (int32_t)(((int16_t)data[6] << 8) | data[7]);
		gyro_bias[1] += (int32_t)(((int16_t)data[8] << 8) | data[9]);
		gyro_bias[2] += (int32_t)(((int16_t)data[10] << 8) | data[11]);
	}

	accel_bias[0] /= (int32_t)packet_count; // Normalize sums to get average count biases
	accel_bias[1] /= (int32_t)packet_count;
	accel_bias[2] /= (int32_t)packet_count;
	gyro_bias[0] /= (int32_t)packet_count;
	gyro_bias[1] /= (int32_t)packet_count;
	gyro_bias[2] /= (int32_t)packet_count;

	// Remove gravity from the z-axis accelerometer bias calculation
	if (accel_bias[2] > 0L) 
	{ 
		accel_bias[2] -= (int32_t)accelsensitivity; 
	}
	else 
	{ 
		accel_bias[2] += (int32_t)accelsensitivity;
	}

	// Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
	data[0] = (-gyro_bias[0] / 4 >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
	data[1] = (-gyro_bias[0] / 4) & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
	data[2] = (-gyro_bias[1] / 4 >> 8) & 0xFF;
	data[3] = (-gyro_bias[1] / 4) & 0xFF;
	data[4] = (-gyro_bias[2] / 4 >> 8) & 0xFF;
	data[5] = (-gyro_bias[2] / 4) & 0xFF;

	/*
	// Push gyro biases to hardware registers
	writeRegister8(MPU6050_ADDRESS, XG_OFFS_USRH, data[0]);// might not be supported in MPU6050
	writeRegister8(MPU6050_ADDRESS, XG_OFFS_USRL, data[1]);
	writeRegister8(MPU6050_ADDRESS, YG_OFFS_USRH, data[2]);
	writeRegister8(MPU6050_ADDRESS, YG_OFFS_USRL, data[3]);
	writeRegister8(MPU6050_ADDRESS, ZG_OFFS_USRH, data[4]);
	writeRegister8(MPU6050_ADDRESS, ZG_OFFS_USRL, data[5]);
	*/

	gyroBias[0] = (float)gyro_bias[0] / (float)gyrosensitivity; // construct gyro bias in deg/s for later manual subtraction
	gyroBias[1] = (float)gyro_bias[1] / (float)gyrosensitivity;
	gyroBias[2] = (float)gyro_bias[2] / (float)gyrosensitivity;
	

	// Construct the accelerometer biases for push to the hardware accelerometer bias registers. These registers contain
	// factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold
	// non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature
	// compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB per g, so that
	// the accelerometer biases calculated above must be divided by 8.

	int32_t accel_bias_reg[3] = { 0, 0, 0 }; // A place to hold the factory accelerometer trim biases
	readBytes(MPU6050_ADDRESS, XA_OFFSET_H, 2, &data[0]); // Read factory accelerometer trim values
	accel_bias_reg[0] = (int16_t)((int16_t)data[0] << 8) | data[1];
	readBytes(MPU6050_ADDRESS, YA_OFFSET_H, 2, &data[0]);
	accel_bias_reg[1] = (int16_t)((int16_t)data[0] << 8) | data[1];
	readBytes(MPU6050_ADDRESS, ZA_OFFSET_H, 2, &data[0]);
	accel_bias_reg[2] = (int16_t)((int16_t)data[0] << 8) | data[1];

	uint32_t mask = 1uL; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
	uint8_t mask_bit[3] = { 0, 0, 0 }; // Define array to hold mask bit for each accelerometer bias axis

	for (ii = 0; ii < 3; ii++) 
	{
		if (accel_bias_reg[ii] & mask) mask_bit[ii] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
	}

	// Construct total accelerometer bias, including calculated average accelerometer bias from above
	accel_bias_reg[0] -= (accel_bias[0] / 8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
	accel_bias_reg[1] -= (accel_bias[1] / 8);
	accel_bias_reg[2] -= (accel_bias[2] / 8);

	data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
	data[1] = (accel_bias_reg[0]) & 0xFF;
	data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
	data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
	data[3] = (accel_bias_reg[1]) & 0xFF;
	data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
	data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
	data[5] = (accel_bias_reg[2]) & 0xFF;
	data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

	/*
	// Push accelerometer biases to hardware registers
	writeRegister8(MPU6050_ADDRESS, XA_OFFSET_H, data[0]); // might not be supported in MPU6050
	writeRegister8(MPU6050_ADDRESS, XA_OFFSET_L_TC, data[1]);
	writeRegister8(MPU6050_ADDRESS, YA_OFFSET_H, data[2]);
	writeRegister8(MPU6050_ADDRESS, YA_OFFSET_L_TC, data[3]);
	writeRegister8(MPU6050_ADDRESS, ZA_OFFSET_H, data[4]);
	writeRegister8(MPU6050_ADDRESS, ZA_OFFSET_L_TC, data[5]);
	*/

	// Output scaled accelerometer biases for manual subtraction in the main program
	accelBias[0] = (float)accel_bias[0] / (float)accelsensitivity;
	accelBias[1] = (float)accel_bias[1] / (float)accelsensitivity;
	accelBias[2] = (float)accel_bias[2] / (float)accelsensitivity;
}


void IMU::begin(uint8_t irq_pin)
{
	Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);

	delay(500);

	lpf = FILTER_188HZ;
	gyro_fsr = FSR_500DPS;
	accel_fsr = FSR_4g;

	self_test();

	calibrate();

	writeRegister8(MPU6050_ADDRESS, PWR_MGMT_1, 0x80); // PWR_MGMT_1 : DEVICE_RESET
	delay(100);

	writeRegister8(MPU6050_ADDRESS, PWR_MGMT_1, 0x03); // PWR_MGMT_1 : TEMP_DIS
	delay(20);

	writeRegister8(MPU6050_ADDRESS, DLPF_CONFIG, lpf);  // CONFIG : DLPF_CFG
	writeRegister8(MPU6050_ADDRESS, GYRO_CONFIG, gyro_fsr << 3); // GYRO_CONFIG
	writeRegister8(MPU6050_ADDRESS, ACCEL_CONFIG, accel_fsr << 3); // ACCEL_CONFIG

	writeRegister8(MPU6050_ADDRESS, INT_PIN_CFG, 0x02);
	writeRegister8(MPU6050_ADDRESS, INT_ENABLE, 0x01);  // Enable data ready (bit 0) interrupt

	imuReady = false;

	pinMode(irq_pin, INPUT);
	attachInterruptEx(irq_pin, [this] { this->imuInterrupt(); }, RISING);
}

void IMU::update()
{
	if (imuReady)
	{
		read_raw();
		apply_calibration();
		apply_inversion_and_scale();

		update_fusion();

		imuReady = false;
	}
}

inline void IMU::read_raw()
{
	//sei();

	Wire.beginTransmission(0x68);
	Wire.write(0x3B);
	Wire.endTransmission(I2C_NOSTOP);
	Wire.requestFrom(0x68, 14);

	while (Wire.available() < 14);

	// accel
	data.axis[0] = int16_t((Wire.read() << 8) | Wire.read());
	data.axis[1] = int16_t((Wire.read() << 8) | Wire.read());
	data.axis[2] = int16_t((Wire.read() << 8) | Wire.read());

	// unused temp data
	temperature = int16_t((Wire.read() << 8) | Wire.read());

	// gyro
	data.axis[3] = int16_t((Wire.read() << 8) | Wire.read());
	data.axis[4] = int16_t((Wire.read() << 8) | Wire.read());
	data.axis[5] = int16_t((Wire.read() << 8) | Wire.read());

	//cli();
}

inline void IMU::apply_calibration()
{
	data.axis[0] -= accelBias[0];
	data.axis[1] -= accelBias[1];
	data.axis[2] -= accelBias[2];
	data.axis[3] -= gyroBias[0];
	data.axis[4] -= gyroBias[1];
	data.axis[5] -= gyroBias[2];
}

inline void IMU::apply_inversion_and_scale()
{
	float accel_scale = accel_fsr_div[accel_fsr];
	float gyro_scale = gyro_fsr_div[gyro_fsr];

	data.axis[0] *= invert[0] / accel_scale;
	data.axis[1] *= invert[1] / accel_scale;
	data.axis[2] *= invert[2] / accel_scale;

	data.axis[3] *= invert[0] / gyro_scale;
	data.axis[4] *= invert[1] / gyro_scale;
	data.axis[5] *= invert[2] / gyro_scale;

	// custom adjustments
	// calibration appears broken at least for gyro
	data.axis[3] += 10.4f;
	data.axis[4] += 8.6f;
	data.axis[5] += 8.3f;

	/*
	Serial.print(gyroBias[0] / gyro_scale);
	Serial.print("	");
	Serial.print(gyroBias[1] / gyro_scale);
	Serial.print("	");
	Serial.print(gyroBias[2] / gyro_scale);
	Serial.print("	");
	Serial.print(data.axis[3]);
	Serial.print("	");
	Serial.print(data.axis[4]);
	Serial.print("	");
	Serial.println(data.axis[5]);
	*/
}

void IMU::update_mahony(float ax, float ay, float az, float gx, float gy, float gz)
{

}

void IMU::update_madgwick(float ax, float ay, float az, float gx, float gy, float gz)
{
	float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3];         // short name local variable for readability
	float norm;                                               // vector norm
	float f1, f2, f3;                                         // objetive funcyion elements
	float J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33; // objective function Jacobian elements
	float qDot1, qDot2, qDot3, qDot4;
	float hatDot1, hatDot2, hatDot3, hatDot4;
	float gerrx, gerry, gerrz, gbiasx, gbiasy, gbiasz;        // gyro bias error

															  // Auxiliary variables to avoid repeated arithmetic
	float _halfq1 = 0.5f * q1;
	float _halfq2 = 0.5f * q2;
	float _halfq3 = 0.5f * q3;
	float _halfq4 = 0.5f * q4;
	float _2q1 = 2.0f * q1;
	float _2q2 = 2.0f * q2;
	float _2q3 = 2.0f * q3;
	float _2q4 = 2.0f * q4;
	//float _2q1q3 = 2.0f * q1 * q3;
	//float _2q3q4 = 2.0f * q3 * q4;

	// Normalise accelerometer measurement
	norm = sqrtf(ax * ax + ay * ay + az * az);
	if (norm == 0.0f) return; // handle NaN
	norm = 1.0f / norm;
	ax *= norm;
	ay *= norm;
	az *= norm;

	// Compute the objective function and Jacobian
	f1 = _2q2 * q4 - _2q1 * q3 - ax;
	f2 = _2q1 * q2 + _2q3 * q4 - ay;
	f3 = 1.0f - _2q2 * q2 - _2q3 * q3 - az;
	J_11or24 = _2q3;
	J_12or23 = _2q4;
	J_13or22 = _2q1;
	J_14or21 = _2q2;
	J_32 = 2.0f * J_14or21;
	J_33 = 2.0f * J_11or24;

	// Compute the gradient (matrix multiplication)
	hatDot1 = J_14or21 * f2 - J_11or24 * f1;
	hatDot2 = J_12or23 * f1 + J_13or22 * f2 - J_32 * f3;
	hatDot3 = J_12or23 * f2 - J_33 *f3 - J_13or22 * f1;
	hatDot4 = J_14or21 * f1 + J_11or24 * f2;

	// Normalize the gradient
	norm = sqrtf(hatDot1 * hatDot1 + hatDot2 * hatDot2 + hatDot3 * hatDot3 + hatDot4 * hatDot4);
	hatDot1 /= norm;
	hatDot2 /= norm;
	hatDot3 /= norm;
	hatDot4 /= norm;

	// Compute estimated gyroscope biases
	gerrx = _2q1 * hatDot2 - _2q2 * hatDot1 - _2q3 * hatDot4 + _2q4 * hatDot3;
	gerry = _2q1 * hatDot3 + _2q2 * hatDot4 - _2q3 * hatDot1 - _2q4 * hatDot2;
	gerrz = _2q1 * hatDot4 - _2q2 * hatDot3 + _2q3 * hatDot2 - _2q4 * hatDot1;

	// Compute and remove gyroscope biases
	gbiasx = gerrx * deltat * zeta;
	gbiasy = gerry * deltat * zeta;
	gbiasz = gerrz * deltat * zeta;
	gx -= gbiasx;
	gy -= gbiasy;
	gz -= gbiasz;

	// Compute the quaternion derivative
	qDot1 = -_halfq2 * gx - _halfq3 * gy - _halfq4 * gz;
	qDot2 = _halfq1 * gx + _halfq3 * gz - _halfq4 * gy;
	qDot3 = _halfq1 * gy - _halfq2 * gz + _halfq4 * gx;
	qDot4 = _halfq1 * gz + _halfq2 * gy - _halfq3 * gx;

	// Compute then integrate estimated quaternion derivative
	q1 += (qDot1 - (beta * hatDot1)) * deltat;
	q2 += (qDot2 - (beta * hatDot2)) * deltat;
	q3 += (qDot3 - (beta * hatDot3)) * deltat;
	q4 += (qDot4 - (beta * hatDot4)) * deltat;

	// Normalize the quaternion
	norm = sqrtf(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);    // normalise quaternion
	norm = 1.0f / norm;
	q[0] = q1 * norm;
	q[1] = q2 * norm;
	q[2] = q3 * norm;
	q[3] = q4 * norm;
}


void IMU::update_fusion()// float ax, float ay, float az, float gx, float gy, float gz)
{
	Now = micros();
	deltat = ((Now - lastUpdate) / 1000000.0f); // set integration time by time elapsed since last filter update
	lastUpdate = Now;
	//    if(lastUpdate - firstUpdate > 10000000uL) {
	//      beta = 0.041; // decrease filter gain after stabilized
	//      zeta = 0.015; // increase gyro bias drift gain after stabilized
	//    }
	// Pass gyro rate as rad/s
	update_madgwick(data.ax, data.ay, data.az, data.gx*PI / 180.0f, data.gy*PI / 180.0f, data.gz*PI / 180.0f);

	data.roll = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]), q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);
	data.pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));
	data.yaw = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]), q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);
	
	data.roll *= 180.0f / PI;
	data.pitch *= 180.0f / PI;
	data.yaw *= 180.0f / PI;

	data.roll += angle_offset[0];
	data.pitch += angle_offset[1];
	data.yaw += angle_offset[2];
}
