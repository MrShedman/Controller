

#include "Pins.h"
#include "System.h"
#include "StateStack.h"
#include "StatusBar.h"
#include "NavigationBar.h"
#include "Sticks.h"
#include "BatteryManager.h"
#include "RTC.h"
#include "LCD.h"
#include "TouchController.h"
#include "IMU.h"
#include "Radio.h"
#include "SD.h"
#include "TextGFX.h"
#include "LoadingScreen.h"
#include "Beeper.h"
#include "Icons.h"

StateStack stateStack;
StatusBar statusBar;
NavigationBar navBar;

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

	battery.update();// update_battery();

	sticks_update();

	radio_update();

	imu.update();

	last_user_activity = min(ctp.timeSinceLastTouch(), millis() - last_stick_activity);

	if (last_user_activity > 30000 && battery.state == BatteryManager::DISCHARGING )
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

		navBar.handleTouch(p);
		stateStack.handleTouch(p);
	}

	stateStack.update();
	navBar.draw();
}