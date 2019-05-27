
#include "Arduino.h"
#include <i2c_t3.h>
#include "TouchController.h"
#include "Pins.h"

TouchController ctp;

namespace
{
	volatile bool touchReady = false;
	volatile uint32_t time_of_touch = 0;

	void touchInterrupt()
	{
		touchReady = true;
		time_of_touch = millis();
	}
}

bool TouchController::begin(uint8_t threshhold) 
{
	Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);

	// change threshhold to be higher/lower
	setSensitivity(threshhold);
	writeRegister8(FT6206_REG_CTRL, FT6206_REG_CTRL_KEEP_ACTIVE_MODE);
	writeRegister8(FT6206_REG_POINTRATE, 60); // 60 fps
	writeRegister8(FT6206_REG_G_MODE, FT6206_REG_G_MODE_INTERRUPT_TRIGGER);

	if ((readRegister8(FT6206_REG_VENDID) != 17) || (readRegister8(FT6206_REG_CHIPID) != 6)) return false;

	/*
	Serial.print("Vend ID: "); Serial.println(readRegister8(FT6206_REG_VENDID));
	Serial.print("Chip ID: "); Serial.println(readRegister8(FT6206_REG_CHIPID));
	Serial.print("Firm V: "); Serial.println(readRegister8(FT6206_REG_FIRMVERS));
	Serial.print("Point Rate Hz: "); Serial.println(readRegister8(FT6206_REG_POINTRATE));
	Serial.print("Thresh: "); Serial.println(readRegister8(FT6206_REG_THRESHHOLD));
    */

	pinMode(TOUCH_IRQ_PIN, INPUT);
	attachInterrupt(TOUCH_IRQ_PIN, touchInterrupt, FALLING);
	
	return true;
}

void TouchController::setSensitivity(uint8_t sens)
{
	writeRegister8(FT6206_REG_THRESHHOLD, sens);
}

bool TouchController::touchAvailable()
{
	return touchReady;
}

const Touch& TouchController::getTouch()
{
	if (touchReady)
	{
		Wire.beginTransmission(FT6206_ADDR);
		Wire.write(FT6206_REG_P1_XH);
		Wire.endTransmission(I2C_NOSTOP);
		Wire.requestFrom((uint8_t)FT6206_ADDR, 4u);
		for (uint8_t i = 0; i < 4; i++)
		{
			data[i] = Wire.read();
		}
		Wire.endTransmission();

		currentTouch.point.x = ((data[0] & FT6206_MSB_MASK) << 8) | (data[1] & FT6206_LSB_MASK);
		currentTouch.point.y = ((data[2] & FT6206_MSB_MASK) << 8) | (data[3] & FT6206_LSB_MASK);
		currentTouch.event = Touch::Event((data[0] & FT6206_TOUCH_EVT_FLAG_MASK) >> FT6206_TOUCH_EVT_FLAG_SHIFT);
		currentTouch.time = time_of_touch;

		touchReady = false;
	}

	return currentTouch;
}


uint8_t TouchController::readRegister8(uint8_t reg) 
{
	uint8_t data;
	Wire.beginTransmission((uint8_t)FT6206_ADDR);
	Wire.write(reg);
	Wire.endTransmission(I2C_NOSTOP);
	Wire.requestFrom((uint8_t)FT6206_ADDR, (uint8_t)1);
	data = Wire.read();
	
	//Serial.print("$"); Serial.print(reg, HEX); 
	//Serial.print(": 0x"); Serial.println(x, HEX);
	
	return data;
}

void TouchController::writeRegister8(uint8_t reg, uint8_t val) 
{
    Wire.beginTransmission((uint8_t)FT6206_ADDR);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}