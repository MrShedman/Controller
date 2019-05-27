#pragma once

#include <Arduino.h>

class StatusBar
{
public:

	StatusBar() 
		:
		m_battery_bitmap(UINT16_MAX),
		m_radio_bitmap(UINT16_MAX),
		m_sdcard_bitmap(UINT16_MAX)
	{}

	void update();

private:

	void printDigitsGFX(uint8_t digits);

	void updateTime();
	void updateBattery();
	void updateRadio();
	void updateSDCard();

	uint16_t m_battery_bitmap;
	uint16_t m_radio_bitmap;
	uint16_t m_sdcard_bitmap;
};

extern StatusBar statusBar;