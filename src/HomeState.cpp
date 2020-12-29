
#include <Arduino.h>
#include "HomeState.h"
#include "LCD.h"
#include "TextGFX.h"
#include "StateStack.h"
#include "IMU.h"
#include "System.h"
#include "Pins.h"
#include "Radio.h"
#include "Payloads.h"
#include "Sticks.h"
#include "Beeper.h"
#include "BatteryManager.h"

extern StateStack stateStack;
extern volatile bool card_detect;
extern uint32_t dt;

void HomeState::setup() 
{
	m_update_rate = 30;

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
	LOG();
	m_container.handleTouch(touch);
}

void HomeState::update()
{
	if (m_timer > 1e3 / m_update_rate)
	{
		int16_t x = 5;
		int16_t y = 35;
		int16_t dy = 17;

		/*

		LOG();
		label0.setCursor(x, y);
		label0.print(armedStateTable[ackPayload.armed_status].name);
		label0.print("  ");
		label0.print(ackPayload.bat_voltage);
		label0.print("V");
		label0.draw(m_force_redraw);

		LOG();
		label1.setCursor(x, y+=dy);
		label1.print(ackPayload.packets_per_second);
		label1.print("pps  ");
		label1.print(radio.ackCounter(1000));
		label1.print("aps");
		label1.draw(m_force_redraw);
		
		*/

		LOG();
		label2.setCursor(x, y += dy);
		label2.print(battery.voltage);
		label2.print("V ");
		label2.print(battery.getState());
		label2.draw(m_force_redraw);
		
		LOG();
		label3.setCursor(x, y += dy);
		label3.print(s_throttle.velocity);
		label3.draw(m_force_redraw);

		LOG();
		label4.setCursor(x, y += dy);
		label4.print(1e6 / dt);
		label4.draw(m_force_redraw);

		LOG();
		label5.setCursor(x, y += dy);
		label5.print(battery.percent);
		label5.print("%");

		uint32_t hr = battery.time_remaining / 60;
		uint32_t min = battery.time_remaining - (hr * 60);

		LOG();
		label5.setCursor(x, y += dy);
		label5.print(hr);
		label5.print("h");
		label5.print(min);
		label5.print("m");
		label5.draw(m_force_redraw);

		m_timer = 0;
	}

	LOG();
	m_container.draw(m_force_redraw);

	m_force_redraw = false;
}