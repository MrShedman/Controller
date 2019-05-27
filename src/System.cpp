#include "System.h"
#include "Pins.h"
#include "elapsedMillis.h"
#include "RingBuffer.h"
#include "LCD.h"
#include "TouchController.h"
#include "BatteryManager.h"
#include "Sticks.h"

IntervalTimer hapTimer;

bool haptic_is_on = false;
uint32_t last_user_activity = 0;

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

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void updateScreenBrightness()
{
	if (battery.state == BatteryManager::DISCHARGING)
	{
		last_user_activity = min(ctp.timeSinceLastTouch(), millis() - last_stick_activity);
		last_user_activity = min(last_user_activity, millis() - battery.last_plugged_in);
	}
	else
	{
		last_user_activity = 0;
	}

	if (last_user_activity > 30000)
	{
		if (last_user_activity > 60000)
		{
			display.setMode(LCD::Off);
		}
		else
		{
			display.setMode(LCD::Dimmed);
		}
	}
	else
	{
		display.setMode(LCD::Full);
	}
}