#include "System.h"

#include "LCD.h"
#include "TouchController.h"
#include "BatteryManager.h"
#include "Sticks.h"


uint32_t last_user_activity = 0;

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