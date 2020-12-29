
#include "Pins.h"
#include "logger_serial.h"
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
TimePayload tpayload; 

LoggerSerial logger;

void setup(void) 
{
	display.begin(LCD_CS_PIN, LCD_CD_PIN, LCD_WR_PIN, LCD_RD_PIN, LCD_RST_PIN, LCD_LITE_PIN);

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
	pinMode(BAT_STS_PIN, INPUT);
	pinMode(BAT_CHG_PIN, INPUT_PULLDOWN);
	//pinMode(BAT_LVL_PIN, INPUT);
	pinMode(BEEP_PIN, OUTPUT);
	pinMode(HAP_PIN, OUTPUT);
    
	digitalWriteFast(HAP_PIN,HIGH);
	digitalWriteFast(BEEP_PIN,HIGH);
    
	Serial.begin(115200);

	radio.begin(Radio::Transmit);

	payload.reset();
	ackPayload.reset();

	radio.setPayload(&tpayload, Radio::Payload::Normal);
	radio.setPayload(&ackPayload, Radio::Payload::Ack);

	if (!ctp.begin(TOUCH_IRQ_PIN, 40)) 
	{  // pass in 'sensitivity' coefficient
		Serial.println("Couldn't start FT6206 touchscreen controller");
		while (1);
	}

	imu.begin(IMU_IRQ_PIN);
    
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

QuadPayload bug_payload;
bool radio_bug = false;
unsigned long lastTick = 0;

void loop()
{
	unsigned long now = millis();
	
	if (now - lastTick >= 1000) 
	{
		tpayload.seconds++;

		if (tpayload.seconds >= 60)
		{
			tpayload.seconds = 0;
			tpayload.minutes++;
		}
		if (tpayload.minutes >= 60)
		{
			tpayload.minutes = 0;
			tpayload.hours++;
		}

		lastTick = now;
	}

	t1 = micros();

	syncRTC();

	battery.update();

	sticks_update();

	payload.throttle = s_throttle.value;
	payload.roll = s_roll.value;
	payload.pitch = s_pitch.value;
	payload.yaw = s_yaw.value;

	if (payload.throttle < 1000 || payload.throttle > 2000 ||
		payload.roll < 1000 || payload.roll > 2000 ||
		payload.pitch < 1000 || payload.pitch > 2000 ||
		payload.yaw < 1000 || payload.yaw > 2000)
	{
		bug_payload = payload;
		radio_bug = true;
	}

	if (radio_bug)
	{
		beeper(BEEPER_GYRO_CALIBRATED);
		Serial.print("Radio bug!!!");
		Serial.print(",");
		Serial.print(bug_payload.throttle);
		Serial.print(",");
		Serial.print(bug_payload.roll);
		Serial.print(",");
		Serial.print(bug_payload.pitch);
		Serial.print(",");
		Serial.println(bug_payload.yaw);
		delay(100);

		return;
	}

	radio.update();

	openCard();

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

	//DebugPrintln("stateStack.update()");
	stateStack.update();
	navBar.draw();

	statusBar.update();

	t2 = micros();
	dt = t2 - t1;
}