#include "Arduino.h"
#include "LoadingScreen.h"
#include "LCD.h"
#include "TextGFX.h"

IntervalTimer screenTimer;

volatile uint8_t percent_complete;
uint8_t last_percent_complete;
uint32_t load_end_time;
uint32_t load_start_time;
Point percent_cursor;

const uint32_t load_duration = 1400000;
const uint8_t refresh_rate = 30;

bool finishedLoading()
{
	return percent_complete == 101;
}

void loading_timer()
{
	percent_complete = map(micros(), load_start_time, load_end_time, 0, 100);

	percent_complete = constrain(percent_complete, 0, 100);

	if (percent_complete != last_percent_complete)
	{
		last_percent_complete = percent_complete;

		display.fillRect(Rect(13, 153, map(percent_complete, 0, 100, 0, 214), 14), WHITE, false);
		display.fillRect(Rect(percent_cursor.x, percent_cursor.y, 30, 20), WHITE, false);

		textgfx.setCursor(percent_cursor.x, percent_cursor.y);
		textgfx.print(percent_complete);
		textgfx.print("%");

		if (percent_complete == 100)
		{
			screenTimer.end();
			display.fillScreen(BLACK);
			percent_complete = 101;
		}
	}
}

void beginLoadingScreen()
{
	display.fillScreen(WHITE);
	display.fillRect(Rect(10, 150, 220, 20), BLACK, false);

	textgfx.setTextColor(BLACK);
	textgfx.setTextSize(2);
	textgfx.setCursor(45, 175);
	textgfx.print("Loading...");
	percent_cursor = textgfx.getCursor();

	load_start_time = micros();
	load_end_time = load_start_time + load_duration;

	screenTimer.begin(loading_timer, 1e6 / refresh_rate);
}