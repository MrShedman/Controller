
#include "Sticks.h"
#include "Pins.h"

Stick s_throttle, s_roll, s_pitch, s_yaw;
uint32_t last_stick_activity = UINT32_MAX;;

void sticks_begin()
{
	s_throttle = Stick(JOY2V_PIN, 0, 425, 1875, 3327, true);
	s_roll = Stick(JOY1H_PIN, 50, 431, 1849, 3369, false);
	s_pitch = Stick(JOY1V_PIN, 50, 533, 2015, 3481, true);
	s_yaw = Stick(JOY2H_PIN, 50, 564, 2034, 3512, false);
}

void sticks_update()
{
	s_throttle.update();
	s_roll.update();
	s_pitch.update();
	s_yaw.update();

	stick_activity();
}

bool stick_activity()
{
	bool sticks_active = false;
	float threshold = 0.05f;

	if (abs(s_throttle.velocity) > threshold)
	{
		sticks_active = true;
	}
	else if (abs(s_roll.velocity) > threshold)
	{
		sticks_active = true;
	}
	else if (abs(s_pitch.velocity) > threshold)
	{
		sticks_active = true;
	}
	else if (abs(s_yaw.velocity) > threshold)
	{
		sticks_active = true;
	}

	if (sticks_active)
	{
		last_stick_activity = millis();
	}

	return sticks_active;
}

Stick::Stick(uint8_t pin,
	uint8_t expo,
	uint16_t trim_min,
	uint16_t trim_mid,
	uint16_t trim_max,
	bool trim_reverse)
	:
	pin(pin),
	expo(expo),
	trim_min(trim_min),
	trim_mid(trim_mid),
	trim_max(trim_max),
	trim_reverse(trim_reverse)
{
	//pinMode(pin, INPUT);
	sample_period = 20;

	update();
}

void Stick::update()
{
	value = analogRead(pin);
	apply_trims();
	apply_expo();

	uint32_t now = millis();

	if (now - prev_time > sample_period)
	{
		velocity = float(value - prev_value) / float(now - prev_time);

		prev_value = value;
		prev_time = now;
	}
}

void Stick::apply_trims()
{
	uint16_t min = 1000;
	uint16_t max = 2000;
	uint16_t half = (max - min) / 2;
	uint16_t temp = constrain(value, trim_min, trim_max);

	if (temp < trim_mid)
	{
		temp = map(temp, trim_min, trim_mid, min, half + min);
	}
	else
	{
		temp = map(temp, trim_mid, trim_max, half + min, max);
	}

	value = (trim_reverse ? max - temp + min : temp);
}

void Stick::apply_expo()
{
	if (expo <= 0) return;

	float x = (float)map(value, 1000, 2000, -1000, 1000) / 1000.0f;
	float a = (float)(100 - expo) / 100.0f;
	float b = (float)expo / 100.0f;

	float y = a*x + b*pow(x, 3);

	value = map(y * 1000, -1000, 1000, 1000, 2000);
}