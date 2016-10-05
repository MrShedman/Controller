#include "System.h"
#include "Pins.h"

IntervalTimer buzTimer;
IntervalTimer hapTimer;

bool buzzer_is_on = false;
bool haptic_is_on = false;

float bat_voltage = 0.0f;

void buzzerOff()
{
	digitalWriteFast(BUZ_PIN, HIGH);
	buzTimer.end();
	buzzer_is_on = false;
}

void buzzerOn(uint32_t duration)
{
	if (!buzzer_is_on)
	{
		digitalWriteFast(BUZ_PIN, LOW);
		buzTimer.begin(buzzerOff, duration);
		buzzer_is_on = true;
	}
}

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
	bat_voltage = ((3.3 * (float)analogRead(BAT_LVL_PIN) / 4096) / 0.5833f) * 0.05f + bat_voltage * 0.95f;
}