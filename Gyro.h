
#ifndef __GYRO_H__
#define __GYRO_H__

#include <i2c_t3.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// *          |   ACCELEROMETER    |           GYROSCOPE
// * DLPF_CFG | Bandwidth | Delay  | Bandwidth | Delay  | Sample Rate
// * ---------+-----------+--------+-----------+--------+-------------
// * 0        | 260Hz     | 0ms    | 256Hz     | 0.98ms | 8kHz
// * 1        | 184Hz     | 2.0ms  | 188Hz     | 1.9ms  | 1kHz
// * 2        | 94Hz      | 3.0ms  | 98Hz      | 2.8ms  | 1kHz
// * 3        | 44Hz      | 4.9ms  | 42Hz      | 4.8ms  | 1kHz
// * 4        | 21Hz      | 8.5ms  | 20Hz      | 8.3ms  | 1kHz
// * 5        | 10Hz      | 13.8ms | 10Hz      | 13.4ms | 1kHz
// * 6        | 5Hz       | 19.0ms | 5Hz       | 18.6ms | 1kHz
// * 7        |   -- Reserved --   |   -- Reserved --   | Reserved
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * GYRO_CONFIG | Full Scale Range 
// * ------------+------------------
// * 0           | +/- 250 degrees/sec
// * 1           | +/- 500 degrees/sec
// * 2           | +/- 1000 degrees/sec
// * 3           | +/- 2000 degrees/sec
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * ACCEL_CONFIG | Full Scale Range
// * -------------+------------------
// * 0            | +/- 2g
// * 1            | +/- 4g
// * 2            | +/- 8g
// * 3            | +/- 16g
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Gyro
{
public:

  enum low_pass_filter_reg : uint8_t
  {
    FILTER_256HZ_NOLPF2 = 0,
    FILTER_188HZ,
    FILTER_98HZ,
    FILTER_42HZ,
    FILTER_20HZ,
    FILTER_10HZ,
    FILTER_5HZ,
    FILTER_2100HZ_NOLPF,
    FILTER
  };
    
  enum gyro_full_scale_range_reg : uint8_t
  {
    FSR_250DPS = 0,
    FSR_500DPS,
    FSR_1000DPS,
    FSR_2000DPS,
    NUM_GYRO_FSR
  };

  enum accel_full_scale_range_reg : uint8_t
  {
    FSR_2g = 0,
    FSR_4g,
    FSR_8g,
    FSR_16g,
    NUM_ACCEL_FSR
  };
   
  uint8_t lpf;
  uint8_t gyro_fsr;
  uint8_t accel_fsr;

  const float gyro_fsr_div[4] = {131.0, 65.5, 32.8, 16.4};
  const float accel_fsr_div[4] = {16384, 8192, 4096, 2048};

  struct gyro_data
  {
    float pitch;
    float roll;
    float yaw;
  };

  struct accel_data
  {
    float x;
    float y;
    float z;
  };
  
  void begin()
  {
    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);
    
    delay(500);
    
    lpf = 2;
    gyro_fsr = 1;
    accel_fsr = 1;
    
    i2c_write_reg(0x68, 0x6B, 0x80); // PWR_MGMT_1 : DEVICE_RESET
    delay(100);
    
    i2c_write_reg(0x68, 0x6B, 0x03); // PWR_MGMT_1 : TEMP_DIS
    delay(20);
    
    i2c_write_reg(0x68, 0x1A, lpf);  // CONFIG : DLPF_CFG
    i2c_write_reg(0x68, 0x1B, gyro_fsr << 3); // GYRO_CONFIG
    i2c_write_reg(0x68, 0x1C, accel_fsr << 3); // ACCEL_CONFIG
    
    x_out = &axis[1 - 1];
    y_out = &axis[2 - 1];
    z_out = &axis[3 - 1];

    pitch_out = &axis[1 + 3 - 1];
    roll_out = &axis[2 + 3  - 1];
    yaw_out = &axis[3 + 3  - 1];

    invert[1 - 1] = -1;
    invert[2 - 1] = -1;
    invert[3 - 1] = -1;
  }

  void update()
  {
    read_raw();
    apply_calibration();
    apply_inversion_and_scale();
  }

  inline void read_raw()
  {   
    //sei();
    
    Wire.beginTransmission(0x68);  
    Wire.write(0x3B);              
    Wire.endTransmission();        
    Wire.requestFrom(0x68, 14);
  
    while (Wire.available() < 14);

    // accel
    axis[0] = int16_t((Wire.read() << 8) | Wire.read());
    axis[1] = int16_t((Wire.read() << 8) | Wire.read());
    axis[2] = int16_t((Wire.read() << 8) | Wire.read());

    // unused temp data
    temperature = int16_t((Wire.read() << 8) | Wire.read());

    // gyro
    axis[3] = int16_t((Wire.read() << 8) | Wire.read());
    axis[4] = int16_t((Wire.read() << 8) | Wire.read());
    axis[5] = int16_t((Wire.read() << 8) | Wire.read());

    //cli();
  }
  
  inline void apply_calibration() 
  {
    axis[0] -= axis_calibration[0];
    axis[1] -= axis_calibration[1];
    axis[2] -= axis_calibration[2];
    axis[3] -= axis_calibration[3];
    axis[4] -= axis_calibration[4];
    axis[5] -= axis_calibration[5];
  }
  
  inline void apply_inversion_and_scale() 
  {
    float accel_scale = accel_fsr_div[accel_fsr];
    float gyro_scale = gyro_fsr_div[gyro_fsr];
    
    axis[0] *= invert[0] / accel_scale;
    axis[1] *= invert[1] / accel_scale;
    axis[2] *= invert[2] / accel_scale;

    axis[3] *= invert[0] / gyro_scale;
    axis[4] *= invert[1] / gyro_scale;
    axis[5] *= invert[2] / gyro_scale;
  }

  void calibrate(uint16_t cal_count = 100)
  {
    uint32_t loop_start_time = micros();
        
    for (int i = 0; i < cal_count; i++) 
    {
      read_raw();
      
      axis_calibration[0] += axis[0];
      axis_calibration[1] += axis[1];
      axis_calibration[2] += axis[2];
      axis_calibration[3] += axis[3];
      axis_calibration[4] += axis[4];
      axis_calibration[5] += axis[5];
      
      while(micros() - loop_start_time  < 1e6 / 250);

      loop_start_time = micros();  
    }  

    axis_calibration[0] /= cal_count;
    axis_calibration[1] /= cal_count;
    axis_calibration[2] /= cal_count;
    axis_calibration[3] /= cal_count;
    axis_calibration[4] /= cal_count;
    axis_calibration[5] /= cal_count;
  }

  gyro_data getGyroData()
  {
    return {*pitch_out, *roll_out, *yaw_out};
  }

  accel_data getAccelData()
  {
    return {*x_out, *y_out, *z_out};
  }
  
private:

  float* pitch_out;
  float* roll_out;
  float* yaw_out;

  float* x_out;
  float* y_out;
  float* z_out;
  
  float axis[6];
  float axis_calibration[6];
  int8_t invert[6];

  int16_t temperature;
    
  void i2c_write_reg(int address, uint8_t  reg, uint8_t  val) 
  {
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
  }  
};

#endif
