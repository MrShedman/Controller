#pragma once

#include "Arduino.h"

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

	uint8_t pin;
	uint16_t value;
	float velocity;
	uint8_t expo;
	uint16_t trim_min;
	uint16_t trim_mid;
	uint16_t trim_max;
	bool trim_reverse;

private:

	uint32_t sample_period;
	uint32_t prev_time;
	uint16_t prev_value;
};

void sticks_begin();

void sticks_update();

bool stick_activity();

extern uint32_t last_stick_activity;
extern Stick s_throttle, s_roll, s_pitch, s_yaw;