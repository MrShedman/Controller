
#include "Pins.h"
#include "SDCard.h"
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
#include "Payloads.h"
#include "TextGFX.h"
#include "LoadingScreen.h"
#include "Beeper.h"
#include "Icons.h"

StateStack stateStack;

QuadPayload payload;
QuadAckPayload ackPayload;

void setup(void) 
{
	display.begin();

	beginLoadingScreen();

	analogReadRes(12);
	analogReadAveraging(32);
  
	pinMode(SD_CS_PIN, OUTPUT);
	digitalWriteFast(SD_CS_PIN, LOW);

	pinMode(SD_IRQ_PIN, INPUT_PULLUP);
	attachInterrupt(SD_IRQ_PIN, card_detect_interrupt, CHANGE);
	delayMicroseconds(card_delay);
	//card_interrupt = true;
  	//openCard();
	
	pinMode(POW_OFF_PIN, OUTPUT);

	battery.init();

	//pinMode(BAT_LVL_PIN, INPUT);

	pinMode(HAP_PIN, OUTPUT);
    
	digitalWriteFast(HAP_PIN,HIGH);

    
	Serial.begin(115200);

	radio.begin();

	payload.reset();
	ackPayload.reset();

	radio.setPayload(&payload, Radio::Payload::Normal);
	radio.setPayload(&ackPayload, Radio::Payload::Ack);

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

uint32_t t1 = 0;
uint32_t t2 = 0;
uint32_t dt = 0;

void loop()
{
	t1 = micros();

	syncRTC();

	battery.update();

	sticks_update();

	payload.throttle = s_throttle.value;
	payload.roll = s_roll.value;
	payload.pitch = s_pitch.value;
	payload.yaw = s_yaw.value;

	radio.update();

	//openCard();

	imu.update();

	updateScreenBrightness();

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

	statusBar.update();

	t2 = micros();
	dt = t2 - t1;
}