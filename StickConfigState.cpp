#include "Arduino.h"
#include "StickConfigState.h"
#include "LCD.h"
#include "TextGFX.h"
#include "StateStack.h"
#include "System.h"
#include "Beeper.h"
#include "Numpad.h"
#include "BatteryManager.h"

extern StateStack stateStack;

void StickConfigState::setup()
{
	//m_series4.fill(s_yaw.value);

	m_series1.setRange(3.7f, 4.2f);
	m_series2.setRange(-1000, 1000);
//	m_series3.setRange(-500, 500);

	//m_series1.setRange(Stick::min, Stick::max);
	//m_series2.setRange(Stick::min, Stick::max);
	//m_series3.setRange(Stick::min, Stick::max);
	//m_series4.setRange(Stick::min, Stick::max);

	m_graph.setSize(Rect(15, 40, 210, 229));

	m_graph.pack(&m_series1);
	m_graph.pack(&m_series2);
	//m_graph.pack(&m_series3);
	//m_graph.pack(&m_series4);

	m_series2.m_color = 0x07E0;
	m_series3.m_color = 0x001F;
	m_series4.m_color = 0xFFE0;

	Serial.println("StickConfigState succesfully Initialised");
}

void StickConfigState::handleTouch(const Touch& touch)
{
	
}

void StickConfigState::update()
{
	if (timer > 500)
	{
		m_graph.clear();

		m_series1.push(battery.voltage);
		m_series2.push(battery.time_remaining);

		m_graph.draw();

		timer = 0;
	}

	m_force_redraw = false;
}