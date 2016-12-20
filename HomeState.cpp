
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
#include "BatteryManager.h"

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

		stateStack.pushState(State::RadioConfig); 
	});

	but2.setCallback([]()
	{
		beeper(BEEPER_SHORT);

		stateStack.pushState(State::StickConfig);
	});

	but3.setCallback([]()
	{
		beeper(BEEPER_SHORT);

		stateStack.pushState(State::IMUConfig);
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

		const IMU::sensor_data& sensor_data = imu.get_data();
		
		label0.setCursor(5, 30);
		label0.print(armedStateTable[ackPayload.armed_status].name);
		label0.print("  ");
		label0.print(ackPayload.bat_voltage);
		label0.print("V");
		label0.draw(m_force_redraw);

		label1.setCursor(5, 45);
		label1.print(ackPayload.packets_per_second);
		label1.print("pps  ");
		label1.print(acks_per_second);
		label1.print("aps");
		label1.draw(m_force_redraw);
		
		label2.setCursor(5, 75);
		label2.print(battery.voltage);
		label2.print("V ");
		label2.print(battery.getState());
		label2.draw(m_force_redraw);

		label3.setCursor(5, 90);
		label3.print("Card Detected: ");
		label3.print(card_detect ? "Yes" : "No");
		label3.draw(m_force_redraw);

		label4.setCursor(5, 105);
		label4.print(s_throttle.velocity);
		label4.draw(m_force_redraw);

		label5.setCursor(5, 120);
		label5.print(battery.percent);
		label5.print("%");

		int16_t hr = battery.time_remaining / 60;
		int16_t min = battery.time_remaining - (hr * 60);

		label5.setCursor(5, 135);
		label5.print(hr);
		label5.print("h");
		label5.print(min);
		label5.print("m");
		label5.draw(m_force_redraw);

		//Serial.print("t: ");
		//Serial.println(micros() - loop_start_time);
	}

	m_container.draw(m_force_redraw);

	m_force_redraw = false;
}