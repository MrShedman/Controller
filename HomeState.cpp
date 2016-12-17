
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

extern StateStack stateStack;
extern volatile bool card_detect;

void HomeState::setup() 
{
	but1.setShape(Rect(15, 160, 210, 30));
	but1.setText("Radio Settings");

	but2.setShape(Rect(15, 200, 210, 30));
	but2.setText("Stick Settings");

	but3.setShape(Rect(15, 240, 210, 30));
	but3.setText("IMU Settings");

	but1.setCallback([]() 
	{		
		beeper(BEEPER_SHORT);

		stateStack.requestStateChange(State::RadioConfig); 
	});

	but2.setCallback([]()
	{
		beeper(BEEPER_SHORT);

		stateStack.requestStateChange(State::StickConfig);
	});

	but3.setCallback([]()
	{
		beeper(BEEPER_SHORT);

		stateStack.requestStateChange(State::IMUConfig);
	});

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
		
		uint32_t temp_t = micros();
		
		Serial.print("d: ");
		Serial.print(temp_t - loop_start_time);
		Serial.print("	");

		const IMU::sensor_data& sensor_data = imu.get_data();

		textgfx.setCursor(5, 30);
		textgfx.print(armedStateTable[ackPayload.armed_status].name);
		
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
		textgfx.print(battery.voltage);
		textgfx.print("V ");
		textgfx.print(batteryStateTable[battery.state].name);

		textgfx.setCursor(5, 90);
		textgfx.print("Card Detected: ");
		textgfx.print(card_detect ? "Yes" : "No");

		textgfx.setCursor(5, 105);
		textgfx.print(s_throttle.velocity);

		textgfx.setCursor(5, 120);
		textgfx.print(battery.percent);
		textgfx.print("%");

		Serial.print("t: ");
		Serial.println(micros() - temp_t);
	}

	m_container.draw(m_force_redraw);

	m_force_redraw = false;
}