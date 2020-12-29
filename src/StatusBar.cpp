#include "StatusBar.h"

#include <TimeLib.h>
#include "Icons.h"
#include "LCD.h"
#include "TextGFX.h"
#include "Radio.h"
#include "BatteryManager.h"

StatusBar statusBar;
extern volatile bool card_detect;

void StatusBar::update()
{
	updateTime();

	updateBattery();

	updateRadio();

	updateSDCard();
}

void StatusBar::printDigitsGFX(uint8_t digits)
{
	// utility function for digital clock display: prints preceding colon and leading 0
	if (digits < 10) textgfx.print('0');
	textgfx.print(digits);
}

void StatusBar::updateTime()
{
	static uint8_t prev_sec = minute() + 1;

	if (prev_sec == minute())
	{
		return;
	}

	prev_sec = minute();

	display.fillRect(Rect(200, 0, 40, 30), BLACK);

	textgfx.setTextSize(1);

	textgfx.setCursor(200, 8);
	printDigitsGFX(hour());
	textgfx.print(":");
	printDigitsGFX(minute());

	textgfx.setCursor(190, 18);
	printDigitsGFX(day());
	textgfx.print("/");
	printDigitsGFX(month());
	textgfx.print("/");
	textgfx.println(year() - 2000);

	textgfx.setTextSize(2);
}

void StatusBar::updateBattery()
{
	uint16_t battery_bitmap = bat_crit;

	if (battery.state == BatteryManager::DISCHARGING)
	{
		if (battery.percent > 95)
		{
			battery_bitmap = bat_100;
		}
		else if (battery.percent > 80)
		{
			battery_bitmap = bat_80;
		}
		else if (battery.percent > 60)
		{
			battery_bitmap = bat_60;
		}
		else if(battery.percent > 40)
		{
			battery_bitmap = bat_40;
		}
		else if (battery.percent > 20)
		{
			battery_bitmap = bat_20;
		}
		else
		{
			battery_bitmap = bat_crit;
		}
	}
	else if (battery.state == BatteryManager::CHARGING)
	{
		if (battery.percent > 95)
		{
			battery_bitmap = bat_charg_100;
		}
		else if (battery.percent > 80)
		{
			battery_bitmap = bat_charg_80;
		}
		else if (battery.percent > 60)
		{
			battery_bitmap = bat_charg_60;
		}
		else if (battery.percent > 40)
		{
			battery_bitmap = bat_charg_40;
		}
		else if (battery.percent > 20)
		{
			battery_bitmap = bat_charg_20;
		}
	}
	else
	{
		battery_bitmap = bat_charged_ext_pow;
	}

	if (battery_bitmap != m_battery_bitmap)
	{
		m_battery_bitmap = battery_bitmap;

		display.drawBitmapFromAtlas(bitmap_atlas, m_battery_bitmap, 170, 5);
	}
}

void StatusBar::updateRadio()
{
	uint16_t radio_bitmap = sig_none;

	if (radio.hasConnection())
	{
		const uint8_t RSSI = radio.getRSSI();

		if (RSSI > 75)
		{
			radio_bitmap = sig_max;
		}
		else if (RSSI > 50)
		{
			radio_bitmap = sig_high;
		}
		else if (RSSI > 25)
		{
			radio_bitmap = sig_med;
		}
		else
		{
			radio_bitmap = sig_low;
		}
	}

	if (radio_bitmap != m_radio_bitmap)
	{
		m_radio_bitmap = radio_bitmap;

		display.drawBitmapFromAtlas(bitmap_atlas, m_radio_bitmap, 142, 5);
	}
}

void StatusBar::updateSDCard()
{
	if (card_detect != m_sdcard_bitmap)
	{
		m_sdcard_bitmap = card_detect;

		if (m_sdcard_bitmap)
		{
			display.drawBitmapFromAtlas(bitmap_atlas, sd_card, 120, 5);
		}
		else
		{
			Rect r = getBitmapRect(sd_card);
			r.x = 120;
			r.y = 5;

			display.fillRect(r, BLACK);
		}
	}
}