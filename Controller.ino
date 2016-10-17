

#include "Pins.h"
#include "System.h"
#include "StateStack.h"
#include "StatusBar.h"
#include "Sticks.h"
#include "RTC.h"
#include "LCD.h"
#include "TouchController.h"
#include "IMU.h"
#include "Radio.h"
#include "SD.h"
#include "TextGFX.h"
#include "LoadingScreen.h"
#include "Beeper.h"
#include "icons.c"

StateStack stateStack;
StatusBar statusBar;

extern float bat_voltage;

uint32_t loop_start_time = 0;
uint32_t last_user_activity = 0;
uint32_t statestack_update_time = 0;
const uint32_t statestack_update_rate = 60;

volatile bool card_detect = false;
volatile uint32_t card_delay = 0;

void card_detect_interrupt()
{
  card_detect = !digitalReadFast(SD_IRQ_PIN);

  if (card_detect && (micros() - card_delay) > 1e6)
  {
//   openCard();
    card_delay = micros();
  }
}

void setup(void) 
{
	display.begin();

	beginLoadingScreen();

	analogReadRes(12);
	analogReadAveraging(32);
  
	pinMode(10, OUTPUT);
	digitalWriteFast(10, LOW);

	pinMode(SD_IRQ_PIN, INPUT_PULLUP);
	delayMicroseconds(50);
	card_detect = !digitalReadFast(SD_IRQ_PIN);
  
	if (card_detect)
	{
		//openCard();
	}
   
	attachInterrupt(SD_IRQ_PIN, card_detect_interrupt, CHANGE);
  
	pinMode(POW_OFF_PIN, OUTPUT);
	pinMode(BAT_STS_PIN, INPUT);
	pinMode(BAT_CHG_PIN, INPUT_PULLDOWN);
	//pinMode(BAT_LVL_PIN, INPUT);
	pinMode(BEEP_PIN, OUTPUT);
	pinMode(HAP_PIN, OUTPUT);
    
	digitalWriteFast(HAP_PIN,HIGH);
	digitalWriteFast(BEEP_PIN,HIGH);
    
	Serial.begin(115200);

	radio_begin();

	if (!ctp.begin(40)) 
	{  // pass in 'sensitivity' coefficient
		Serial.println("Couldn't start FT6206 touchscreen controller");
		while (1);
	}

	imu.begin();
    
	setSyncProvider(getTeensy3Time);
  
	if (timeStatus()!= timeSet) 
	{
		Serial.println("Unable to sync with the RTC");
	} 
	else 
	{
		Serial.println("RTC has set the system time");
	}

	syncRTC();

	sticks_begin();

	stateStack.begin();

	while (!finishedLoading())
	{
		delayMicroseconds(50);
	}

	beeper(BEEPER_2_SHORT);

	textgfx.setTextColor(WHITE);
	textgfx.setTextSize(2);
}

void loop()
{
	statusBar.update();

	update_bat_voltage();

	sticks_update();

	radio_update();

	imu.update();

	if ((millis() - loop_start_time) > 1e3 / (70 / 4))
	{
		int16_t size = 15;
		int16_t x0 = 35;
		int16_t y0 = 295;
		int16_t x1 = x0 + size;
		int16_t y1 = y0 + size / 2;
		int16_t x2 = x0 + size;
		int16_t y2 = y0 - size / 2;

		//display.drawTriangle(x0, y0, x1, y1, x2, y2, WHITE);
		//display.drawCircle(120, 295, size / 2, WHITE);
		//display.drawRoundRect(Rect(195 - size/2, 295 - size/2, size, size), 4, WHITE);

		display.drawBitmapFromAtlas(bitmap_atlas, back, 20, 280);
		display.drawBitmapFromAtlas(bitmap_atlas, home, 120 - 44 / 2, 280);
		display.drawBitmapFromAtlas(bitmap_atlas, settings, 185, 280);
	}

	last_user_activity = min(ctp.timeSinceLastTouch(), millis() - last_stick_activity);

	bool usb = digitalReadFast(BAT_CHG_PIN);

	if (last_user_activity > 30000 && !usb)
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

	if (ctp.touchAvailable())
	{
		// Retrieve a point  
		const Touch& p = ctp.getTouch();

		if (p.event == Touch::Event::pressed)
		{
			hapticOn();
		}

		stateStack.handleTouch(p);
	}

	stateStack.update();		
}