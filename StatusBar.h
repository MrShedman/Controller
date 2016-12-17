#pragma once

#include "Arduino.h"

#include "Icons.h"

class StatusBar
{
public:

	void update();

private:

	void printDigitsGFX(uint8_t digits);

	void displayTime();

	uint32_t loop_timer = 0;
	int bat_counter = bat_charged_ext_pow;
	int sig_counter = sig_none;
};