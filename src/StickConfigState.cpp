#include <Arduino.h>
#include "StickConfigState.h"
#include "LCD.h"
#include "TextGFX.h"
#include "StateStack.h"
#include "System.h"
#include "Beeper.h"
#include "Numpad.h"
#include "BatteryManager.h"
#include "Radio.h"
#include "Payloads.h"

extern StateStack stateStack;

void StickConfigState::setup()
{
	m_update_rate = 30;

	rect.x = 100;
	rect.y = 200;
	rect.w = 50;
	rect.h = 50;

	Serial.println("StickConfigState succesfully Initialised");
}

float vel = 0.0f;
const float decay = 0.92f;
int dir = 0;

void StickConfigState::handleTouch(const Touch& touch)
{
	scrollview.handleTouch(touch);

	if (touch.gesture != Touch::Gesture::none)
	{
		//display.fillRect(rect, BLACK);

		const int16_t m = abs(touch.gestureVel);
		vel = touch.gestureVel*3.0f;

		switch (touch.gesture)
		{
		case Touch::Gesture::swipe_down:	rect.move(0, m); dir = 0;	break;
		case Touch::Gesture::swipe_up:		rect.move(0, -m); dir = 0;	break;
		case Touch::Gesture::swipe_left:	rect.move(-m, 0); dir = 1;	break;
		case Touch::Gesture::swipe_right:	rect.move(m, 0); dir = 1;	break;
		}
	}
}

void StickConfigState::update()
{
	if (m_timer > 1e3 / m_update_rate)
	{
		scrollview.draw(m_force_redraw);

		/*
		//display.fillRect(rect, BLACK);

		if (dir == 0)
		{
			rect.move(0, vel);
		}
		else if (dir == 1)
		{
			rect.move(vel, 0);
		}

		vel *= decay;

		//display.fillRect(rect, 0x07E0);

		int16_t x = 5;
		int16_t y = 35;
		int16_t dy = 17;
		
		label0.setCursor(x, y);
		label0.print("Bat voltage: ");
		label0.print(ackPayload.bat_voltage);
		label0.print("V");
		label0.draw(m_force_redraw);

		label1.setCursor(x, y += dy);
		label1.print("MOSFET Temp: ");
		label1.print(ackPayload.temp_mos_avg);
		label1.print("C");
		label1.draw(m_force_redraw);

		label2.setCursor(x, y += dy);
		label2.print("Mot Current: ");
		label2.print(ackPayload.current_motor);
		label2.print("A");
		label2.draw(m_force_redraw);

		//label3.setCursor(x, y += dy);
		//label3.print("RPM: ");
		//label3.print(ackPayload.rpm);
		//label3.draw(m_force_redraw);

		label4.setCursor(x, y += dy);
		label4.print("Tacho: ");
		label4.print(ackPayload.tachometer_abs);
		label4.draw(m_force_redraw);
		
		*/
		m_timer = 0;
	}
	
	m_force_redraw = false;
}