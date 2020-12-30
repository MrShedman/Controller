
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

#include "scheduler.h"
#include "task.h"
#include "time.h"

StateStack stateStack;

QuadPayload payload;
QuadAckPayload ackPayload;
TimePayload tpayload; 

LoggerSerial logger;

//typedef bool (*checkFunction)(const Time& currentTime, const Time& currentDeltaTime);
//typedef void (*taskFunction)(const Time& currentTime);

const Time touch_hz = hertz(60.0);

bool touch_check_func(const Time& currentTime, const Time& currentDeltaTime)
{
	return ctp.touchAvailable();
}

void touch_task_func(const Time& currentTime)
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

const Time imu_hz = hertz(200.0);

bool imu_check_func(const Time& currentTime, const Time& currentDeltaTime)
{
	return imu.available();
}

void imu_task_func(const Time& currentTime)
{
	imu.update();
}

const Time radio_hz = hertz(100.0);

void radio_task_func(const Time& currentTime)
{
	radio.update();
}

const Time state_hz = hertz(60.0);

void state_task_func(const Time& currentTime)
{
	stateStack.update();
}

const Time system_hz = hertz(30.0);

void system_task_func(const Time& currentTime)
{
	openCard();
	updateScreenBrightness();
	syncRTC();
	battery.update();	
	statusBar.update();
	navBar.draw();
}

const Time stick_hz = hertz(60.0);

void stick_task_func(const Time& currentTime)
{
	sticks_update();
}

Task stick_task("stick_task", 		nullptr, 			stick_task_func,	stick_hz,	TASK_PRIORITY_MEDIUM_HIGH);
Task system_task("system_task", 	nullptr, 			system_task_func,	system_hz,	TASK_PRIORITY_LOW);
Task state_task("state_task", 		nullptr, 			state_task_func,  	state_hz,  	TASK_PRIORITY_REALTIME);
Task radio_task("radio_task", 		nullptr, 			radio_task_func,  	radio_hz,  	TASK_PRIORITY_REALTIME);
Task imu_task("imu_task", 			imu_check_func, 	imu_task_func, 	 	imu_hz, 	TASK_PRIORITY_MEDIUM_HIGH);
Task touch_task("touch_task",   	touch_check_func, 	touch_task_func,  	touch_hz,	TASK_PRIORITY_REALTIME);

void init_scheduler()
{
    schedulerInit();
    addTask(&stick_task);
    addTask(&system_task);
    addTask(&state_task);
    addTask(&radio_task);
    addTask(&imu_task);
	addTask(&touch_task);	

    setTaskEnabled(&stick_task, true);
    setTaskEnabled(&system_task, true);
    setTaskEnabled(&state_task, true);
    setTaskEnabled(&radio_task, true);
    setTaskEnabled(&imu_task, true);
    setTaskEnabled(&touch_task, true);
}

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

	init_scheduler();
}

uint32_t t1 = 0;
uint32_t t2 = 0;
uint32_t dt = 0;

QuadPayload bug_payload;
bool radio_bug = false;
unsigned long lastTick = 0;

void loop()
{
	t1 = micros();

	scheduler();

	t2 = micros();
	dt = t2 - t1;
}