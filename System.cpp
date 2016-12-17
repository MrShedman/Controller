#include "System.h"
#include "Pins.h"

IntervalTimer hapTimer;

bool haptic_is_on = false;

void hapticOff()
{
	digitalWriteFast(HAP_PIN, HIGH);
	hapTimer.end();
	haptic_is_on = false;
}

void hapticOn(uint32_t duration)
{
	if (!haptic_is_on)
	{
		digitalWriteFast(HAP_PIN, LOW);
		hapTimer.begin(hapticOff, duration);
		haptic_is_on = true;
	}
}

Battery battery;

void update_battery()
{
	if (digitalReadFast(BAT_STS_PIN))
	{
		battery.state = CHARGING;
	}
	else if (digitalReadFast(BAT_CHG_PIN))
	{
		battery.state = CHARGING;
	}
	else
	{
		battery.state = DISCHARGING;
	}

	const float bv = 0.026f + (3.3 * (float)analogRead(BAT_LVL_PIN) / 4096) / 0.5833f;

	battery.voltage = bv * 0.05f + battery.voltage * 0.95f;

	battery.voltage = constrain(battery.voltage, 3.7, 4.2);

	//float temp = (bat_voltage - 3.7f) * 2.0f * 100.0f;

	float temp = -856 * pow(battery.voltage, 3) + 9930 * pow(battery.voltage, 2) - 38120 * battery.voltage + 48460;

	temp = constrain(temp, 0.0f, 100.0f);

	battery.percent = ceil(temp);
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}