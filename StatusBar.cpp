#include "StatusBar.h"

#include <TimeLib.h>

#include "LCD.h"
#include "TextGFX.h"

extern LCD display;
extern TextGFX textgfx;

void StatusBar::update()
{
	if (millis() - loop_timer > 1000)
	{
		display.drawBitmapFromAtlas(bitmap_atlas, bat_counter++, 173, 5);
		display.drawBitmapFromAtlas(bitmap_atlas, sig_counter++, 145, 5);
		display.drawBitmapFromAtlas(bitmap_atlas, sd_card, 123, 5);

		if (bat_counter >= bat_20)
		{
			bat_counter = bat_charged_ext_pow;
		}

		if (sig_counter >= sig_max)
		{
			sig_counter = sig_none;
		}

		loop_timer = millis();
	}

	displayTime();
}

void StatusBar::printDigitsGFX(uint8_t digits)
{
	// utility function for digital clock display: prints preceding colon and leading 0
	if (digits < 10) textgfx.print('0');
	textgfx.print(digits);
}

void StatusBar::displayTime()
{
	static uint8_t prev_sec = minute() + 1;

	if (prev_sec == minute())
	{
		return;
	}

	prev_sec = minute();

	display.fillRect(Rect(200, 0, 40, 30), BLACK);

	textgfx.setCursor(200, 8);
	textgfx.setTextSize(1);

	// digital clock display of the time
	printDigitsGFX(hour());
	textgfx.print(":");
	printDigitsGFX(minute());
	textgfx.setCursor(190, 18);
	textgfx.print(day());
	textgfx.print("/");
	textgfx.print(month());
	textgfx.print("/");
	textgfx.println(year() - 2000);

	textgfx.setTextSize(2);
}