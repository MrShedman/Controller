#include "System.h"
#include "Pins.h"

IntervalTimer hapTimer;

bool haptic_is_on = false;

uint8_t bat_percent = 0;
float bat_voltage = 0.0f;

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

void update_bat_voltage()
{
	const float bv = 0.026f + (3.3 * (float)analogRead(BAT_LVL_PIN) / 4096) / 0.5833f;

	bat_voltage = bv * 0.05f + bat_voltage * 0.95f;

	bat_voltage = constrain(bat_voltage, 3.7, 4.2);

	//float temp = (bat_voltage - 3.7f) * 2.0f * 100.0f;

	float temp = -856 * pow(bat_voltage, 3) + 9930 * pow(bat_voltage, 2) - 38120 * bat_voltage + 48460;

	temp = constrain(temp, 0.0f, 100.0f);

	bat_percent = ceil(temp);
}