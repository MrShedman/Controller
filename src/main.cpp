#include "sdcard.h"
#include "System.h"
#include "StateStack.h"
#include "StatusBar.h"
#include "NavigationBar.h"
#include "Sticks.h"
#include "BatteryManager.h"
#include "rtc.h"
#include "LCD.h"
#include "TouchController.h"
#include "IMU.h"
#include "Radio.h"
#include "Payloads.h"
#include "TextGFX.h"
#include "LoadingScreen.h"
#include "Beeper.h"
#include "Icons.h"
#include "haptic.h"

StateStack stateStack;

QuadPayload payload;
QuadAckPayload ackPayload;

const uint8_t POW_OFF_PIN = 23;

void setup(void) 
{
	display.begin();

	beginLoadingScreen();

	analogReadRes(12);
	analogReadAveraging(32);
  
	init_sdcard();
	
	pinMode(POW_OFF_PIN, OUTPUT);

	battery.init();

	//pinMode(BAT_LVL_PIN, INPUT);


	init_haptic();

    
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
    
	init_rtc();

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

	sync_rtc();

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
			haptic_on();
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