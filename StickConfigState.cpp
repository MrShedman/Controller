#include "Arduino.h"
#include "StickConfigState.h"
#include "LCD.h"
#include "TextGFX.h"
#include "StateStack.h"
#include "System.h"
#include "Numpad.h"

extern LCD display;
extern TextGFX textgfx;
extern StateStack stateStack;
extern Numpad numpad;

namespace
{
	void cb()
	{
		Serial.println("callback!");

		buzzerOn();

		stateStack.requestStateChange(State::Home);
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

	void cbnum(float p)
	{
		Serial.print("numpad!	");
		Serial.println(p, 10);

		//display.setBrightness(p);
	}
}

void StickConfigState::setup()
{
	button_back.setShape(Rect(15, 40, 210, 30));
	button_back.setText("Back");
	button_back.setCallback(cb);

	m_container.pack(&button_back);

	numpad.setCallback(cbnum);

	Serial.println("StickConfigState succesfully Initialised");
}

void StickConfigState::handleTouch(const Touch& touch)
{
	for (uint8_t i = 0; i < m_container.size(); ++i)
	{
		m_container[i]->handleTouch(touch);
	}

	numpad.handleTouch(touch);
}

void StickConfigState::update()
{
	for (uint8_t i = 0; i < m_container.size(); ++i)
	{
		m_container[i]->draw(m_force_redraw);
	}

	numpad.draw(m_force_redraw);

	m_force_redraw = false;
}