#include <Arduino.h>
#include "VehicleConfigState.h"
#include "LCD.h"
#include "TextGFX.h"
#include "StateStack.h"
#include "System.h"
#include "Beeper.h"
#include "Numpad.h"

extern StateStack stateStack;

namespace
{
	void cb()
	{
		Serial.println("callback!");

		beeper(BEEPER_SHORT);

		stateStack.pushState(State::Home);
	}

	void cbt(bool t)
	{
		Serial.print("callback!");
		Serial.println((t ? "on" : "off"));

		//display.invert(t);
	}

	void cbs(int p)
	{
		Serial.print("percent!");
		Serial.println(p);

		//display.setBrightness(p);
	}

	void cbnum(const String& p)
	{
		Serial.print("numpad!	");
		Serial.println(p);

		//display.setBrightness(p);
	}
}

void VehicleConfigState::setup()
{
	button_back.setShape(Rect(15, 40, 210, 30));
	button_back.setText("Back");
	button_back.setCallback(cb);

	m_container.pack(&button_back);

	numpad.setCallback(cbnum);

	Serial.println("StickConfigState succesfully Initialised");
}

void VehicleConfigState::handleTouch(const Touch& touch)
{
	m_container.handleTouch(touch);

	numpad.handleTouch(touch);
}

void VehicleConfigState::update()
{
	m_container.draw(m_force_redraw);

	numpad.draw(m_force_redraw);

	m_force_redraw = false;
}