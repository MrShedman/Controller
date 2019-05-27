#include "BatteryManager.h"

BatteryManager battery;

const uint8_t BAT_STS_PIN = 22;
const uint8_t BAT_LVL_PIN = A12;
const uint8_t BAT_CHG_PIN = 1;

bool BatteryManager::linreg(float *m, float *b) const
{
	float sumx = 0.0f;
	float sumx2 = 0.0f;
	float sumxy = 0.0f;
	float sumy = 0.0f;
	float sumy2 = 0.0f;

	const float n = log_buffer.size();

	const Log* l = log_buffer.tail();

	for (uint16_t i = 0; i < n; i++)
	{
		sumx += l->time;
		sumx2 += powf(l->time, 2);
		sumxy += l->time * l->value;
		sumy += l->value;
		sumy2 += powf(l->value, 2);

		l = log_buffer.next(l);
	}

	float denom = (n * sumx2 - powf(sumx, 2));

	if (denom == 0.0f)
	{
		// singular matrix. can't solve the problem.
		*m = 0.0f;
		*b = 0.0f;
		return false;
	}

	*m = (n * sumxy - sumx * sumy) / denom;
	*b = (sumy * sumx2 - sumx * sumxy) / denom;

	return true;
}

void BatteryManager::predictRemainingTime()
{
	log_buffer.push(Log(voltage, millis() / 1000.0f));

	if (log_buffer.size() > (log_min_time * 1000 / log_delta))
	{
		float m, b;

		if (linreg(&m, &b))
		{
			float y = (battery.state == DISCHARGING ? 3.7f : 4.2f);

			float x = abs((y - b) / m);

			battery.time_remaining = x / 60;
		}
	}
}

void BatteryManager::init()
{
	pinMode(BAT_STS_PIN, INPUT);
	pinMode(BAT_CHG_PIN, INPUT_PULLDOWN);
}

void BatteryManager::update()
{
	const State old_state = state;
	
	if (digitalReadFast(BAT_STS_PIN))
	{
		state = CHARGED;
	}
	else if (digitalReadFast(BAT_CHG_PIN))
	{
		state = CHARGING;
	}
	else
	{
		state = DISCHARGING;

		if (old_state != DISCHARGING)
		{
			last_plugged_in = millis();
		}
	}

	const float raw_voltage = 0.026f + (3.3 * (float)analogRead(BAT_LVL_PIN) / 4096) / 0.5833f;

	voltage = raw_voltage * 0.05f + voltage * 0.95f;

	voltage = constrain(voltage, 3.7, 4.2);

	float temp = -856 * powf(voltage, 3) + 9930 * powf(voltage, 2) - 38120 * voltage + 48460;

	temp = constrain(temp, 0.0f, 100.0f);

	percent = ceilf(temp);

	if (state != CHARGED)
	{
		if (log_timer > log_delta)
		{
			predictRemainingTime();

			log_timer = 0;
		}
	}
	else
	{
		if (!log_buffer.empty())
		{
			log_buffer.clear();
		}

		battery.time_remaining = 0;
	}
}