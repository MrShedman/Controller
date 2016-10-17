
#include "Arduino.h"
#include "HomeState.h"
#include "LCD.h"
#include "TextGFX.h"
#include "StateStack.h"
#include "IMU.h"
#include "System.h"
#include "Pins.h"
#include "Radio.h"
#include "Sticks.h"
#include "Beeper.h"

extern TextGFX textgfx;
extern StateStack stateStack;
extern float bat_voltage;
extern uint8_t bat_percent;
extern volatile bool card_detect;
extern Stick s_throttle;

namespace
{
	void cb()
	{
		Serial.println("callback!");

		beeper(BEEPER_SHORT);

		stateStack.requestStateChange(State::RadioConfig);
	}

	void cb1()
	{
		Serial.println("callback!");

		beeper(BEEPER_SHORT);

		stateStack.requestStateChange(State::StickConfig);
	}

	void cb2()
	{
		Serial.println("callback!");

		beeper(BEEPER_SHORT);

		stateStack.requestStateChange(State::IMUConfig);
	}


	void cbt(bool t)
	{
		Serial.print("callback!");
		Serial.println((t ? "on" : "off"));

		display.invert(t);
	}

	void cbs(int p)
	{
		Serial.print("percent!");
		Serial.println(p);

		display.setBrightness(p);
	}
}

void HomeState::setup() 
{
	but1.setShape(Rect(15, 160, 210, 30));
	but1.setText("Radio Settings");

	but2.setShape(Rect(15, 200, 210, 30));
	but2.setText("Stick Settings");

	but3.setShape(Rect(15, 240, 210, 30));
	but3.setText("IMU Settings");

	but1.setCallback(cb);
	but2.setCallback(cb1);
	but3.setCallback(cb2);

	m_container.pack(&but1);
	m_container.pack(&but2);
	m_container.pack(&but3);

	Serial.println("HomeState succesfully Initialised");
}

void HomeState::handleTouch(const Touch& touch)
{
	m_container.handleTouch(touch);
}

void HomeState::update()
{
	if (micros() - loop_start_time > 1e6 / (60 / 4))
	{
		loop_start_time = micros();

		display.fillRect(Rect(5, 30, 235, 105), BLACK);

		const IMU::sensor_data& sensor_data = imu.get_data();

		textgfx.setCursor(5, 30);
		if (ackPayload.armed_status == 0)
		{
			textgfx.print("Disarmed");
		}
		else if (ackPayload.armed_status == 1)
		{
			textgfx.print("Pending");
		}
		else if (ackPayload.armed_status == 2)
		{
			textgfx.print("Armed");
		}
		textgfx.print("  ");
		textgfx.print(ackPayload.bat_voltage);
		textgfx.print("V");

		textgfx.setCursor(5, 45);
		textgfx.print(ackPayload.packets_per_second);
		textgfx.print("pps  ");
		textgfx.print(acks_per_second);
		textgfx.print("aps");

		//textgfx.setCursor(5, 60);
		//textgfx.print(ackPayload.roll);
		//textgfx.print(" ");
		//textgfx.print(ackPayload.pitch);
		//textgfx.print(" ");
		//textgfx.print(ackPayload.yaw);
		
		textgfx.setCursor(5, 75);
		textgfx.print(bat_voltage);
		textgfx.print("V ");
		textgfx.print("F: ");
		textgfx.print(digitalReadFast(BAT_STS_PIN) == HIGH ? "Y " : "N ");
		textgfx.print("C: ");
		textgfx.print(digitalReadFast(BAT_CHG_PIN) == HIGH ? "Y " : "N ");

		textgfx.setCursor(5, 90);
		textgfx.print("Card Detected: ");
		textgfx.print(card_detect ? "Yes" : "No");

		textgfx.setCursor(5, 105);
		textgfx.print(s_throttle.velocity);

		textgfx.setCursor(5, 120);
		textgfx.print(bat_percent);
		textgfx.print("%");
	}

	m_container.draw(m_force_redraw);

	m_force_redraw = false;
}