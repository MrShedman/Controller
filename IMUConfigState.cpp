#include "Arduino.h"
#include "IMUConfigState.h"
#include "LCD.h"
#include "TextGFX.h"
#include "StateStack.h"
#include "System.h"
#include "Sticks.h"

extern Stick s_throttle, s_roll, s_pitch, s_yaw;
extern LCD display;
extern TextGFX textgfx;
extern StateStack stateStack;

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

		display.setBrightness(p);
	}
}

void IMUConfigState::setup()
{
	button_back.setShape(Rect(15, 40, 210, 30));
	button_back.setText("Back");
	button_back.setCallback(cb);

	m_container.pack(&button_back);

	m_graph.pack(&m_series1);
	m_graph.pack(&m_series2);
	m_graph.pack(&m_series3);
	m_graph.pack(&m_series4);

	m_series2.color = 0x07E0;
	m_series3.color = 0x001F;
	m_series4.color = 0xFFE0;

	Serial.println("IMUConfigState succesfully Initialised");
}

void IMUConfigState::handleTouch(const Touch& touch)
{
	for (uint8_t i = 0; i < m_container.size(); ++i)
	{
		m_container[i]->handleTouch(touch);
	}
}

void IMUConfigState::update()
{
	for (uint8_t i = 0; i < m_container.size(); ++i)
	{
		m_container[i]->draw(m_force_redraw);
	}

	m_series1.push(s_throttle.value);
	m_series2.push(s_roll.value);
	m_series3.push(s_pitch.value);
	m_series4.push(s_yaw.value);

	m_graph.draw();

	m_force_redraw = false;
}