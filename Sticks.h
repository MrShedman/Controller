#pragma once

#include "Arduino.h"
#include "elapsedMillis.h"

class Stick
{
public:

	Stick(){}

	Stick(uint8_t pin,
		uint8_t expo,
		uint16_t trim_min,
		uint16_t trim_mid,
		uint16_t trim_max,
		bool trim_reverse);

	void update();

	void apply_trims();

	void apply_expo();

	void calc_vel();

	uint8_t pin;
	uint16_t value;
	int16_t velocity;
	uint8_t expo;
	uint16_t trim_min;
	uint16_t trim_mid;
	uint16_t trim_max;
	bool trim_reverse;

	static const uint16_t min = 1000;
	static const uint16_t max = 2000;

private:

	elapsedMillis elapsed_time;
	uint16_t prev_value;
	static const uint32_t sample_period = 50;
};

void sticks_begin();

void sticks_update();

bool stick_activity();

extern uint32_t last_stick_activity;
extern Stick s_throttle, s_roll, s_pitch, s_yaw;