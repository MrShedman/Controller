#include "Arduino.h"
#include "RadioConfigState.h"
#include "LCD.h"
#include "TextGFX.h"
#include "StateStack.h"
#include "System.h"

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

void RadioConfigState::setup()
{
	button_back.setShape(Rect(15, 40, 210, 30));
	button_back.setText("Back");

	toggle_ACK_payloads.setShape(Rect(15, 80, 210, 30));

	slider_channel.setShape(Rect(15, 120, 210, 30));
	slider_data_rate.setShape(Rect(15, 160, 210, 30));
	slider_PA_level.setShape(Rect(15, 200, 210, 30));
	slider_refresh_rate.setShape(Rect(15, 240, 210, 30));

	button_back.setCallback(cb);

	toggle_ACK_payloads.setCallback(cbt);

	slider_channel.setCallback(cbs);
	slider_channel.setSliderPosition(display.getBrightness());
	//slider_data_rate.setCallback(cbs);
	//slider_PA_level.setCallback(cbs);
	//slider_refresh_rate.setCallback(cbs);

	m_container.pack(&button_back);
	m_container.pack(&toggle_ACK_payloads);
	m_container.pack(&slider_channel);
	m_container.pack(&slider_data_rate);
	m_container.pack(&slider_PA_level);
	m_container.pack(&slider_refresh_rate);

	Serial.println("RadioConfigState succesfully Initialised");
}

void RadioConfigState::handleTouch(const Touch& touch)
{
	for (uint8_t i = 0; i < m_container.size(); ++i)
	{
		m_container[i]->handleTouch(touch);
	}
}

void RadioConfigState::update()
{
	for (uint8_t i = 0; i < m_container.size(); ++i)
	{
		m_container[i]->draw(m_force_redraw);
	}

	m_force_redraw = false;
}