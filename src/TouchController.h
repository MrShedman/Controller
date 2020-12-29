
#pragma once

#include <Arduino.h>

#include "Touch.h"
#include "circular_buffer.h"
#include <i2c_t3.h>

#define FT6206_ADDR           	0x38
#define FT6206_REG_NUMTOUCHES	0x02
#define FT6206_REG_MODE 		0x00
#define FT6206_REG_CALIBRATE 	0x02
#define FT6206_REG_WORKMODE 	0x00
#define FT6206_REG_FACTORYMODE 	0x40
#define FT6206_REG_THRESHHOLD 	0x80
#define FT6206_REG_POINTRATE 	0x88
#define FT6206_REG_FIRMVERS 	0xA6
#define FT6206_REG_CHIPID 		0xA3
#define FT6206_REG_VENDID 		0xA8

#define FT6206_TOUCH_EVT_FLAG_PRESS_DOWN 0x00
#define FT6206_TOUCH_EVT_FLAG_LIFT_UP    0x01
#define FT6206_TOUCH_EVT_FLAG_CONTACT    0x02
#define FT6206_TOUCH_EVT_FLAG_NO_EVENT   0x03
#define FT6206_TOUCH_EVT_FLAG_SHIFT      6
#define FT6206_TOUCH_EVT_FLAG_MASK       0xC0

#define FT6206_REG_P1_XH                0x03
#define FT6206_MSB_MASK                 0x0F
#define FT6206_MSB_SHIFT                0
#define FT6206_LSB_MASK                 0xFF
#define FT6206_LSB_SHIFT                0

#define FT6206_REG_CTRL 								0x86
#define FT6206_REG_CTRL_KEEP_ACTIVE_MODE    			0x00
#define FT6206_REG_CTRL_KEEP_AUTO_SWITCH_MONITOR_MODE  	0x01

#define FT6206_REG_G_MODE 					0xA4
#define FT6206_REG_G_MODE_INTERRUPT_POLLING 0x00
#define FT6206_REG_G_MODE_INTERRUPT_TRIGGER 0x01


class TouchController 
{
 public:

	bool begin(uint8_t thresh = 128);  

	void setSensitivity(uint8_t sens);

	void setGestureThreshold(uint8_t thresh);

	const Touch& getTouch();	

	bool touchAvailable();

	uint32_t timeSinceLastTouch() const
	{
		return millis() - currentTouch.time;
	}

private:
  
	void calcGestures();
	void calcGesutreVel(Point dp, uint32_t dt);

	uint8_t data[4];
	Touch currentTouch;
  
	bool gestureLock;
	uint8_t gestureThreshold;

	const static uint16_t m_fifo_length = 64;

	CircularBuffer<Touch, m_fifo_length> touch_fifo;

	void writeRegister8(uint8_t reg, uint8_t val);
	uint8_t readRegister8(uint8_t reg);
};

extern TouchController ctp;