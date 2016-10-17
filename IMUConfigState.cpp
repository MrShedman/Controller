#include "Arduino.h"
#include "IMUConfigState.h"
#include "LCD.h"
#include "TextGFX.h"
#include "StateStack.h"
#include "System.h"
#include "Sticks.h"
#include "Beeper.h"
#include "IMU.h"
#include "Radio.h"

extern IMU imu;
extern Stick s_throttle, s_roll, s_pitch, s_yaw;
extern LCD display;
extern TextGFX textgfx;
extern StateStack stateStack;

namespace
{
	uint8_t m_graph_draw_rate;

	void cb()
	{
		Serial.println("callback!");

		beeper(BEEPER_SHORT);

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
		float scale = (float)p / 100.0f;

		m_graph_draw_rate = 40 * scale;

		m_graph_draw_rate += 1;
	}
}

void IMUConfigState::setup()
{
	m_graph_draw_rate = 20;
	m_graph_last_draw_time = 0;

	button_back.setShape(Rect(15, 40, 210, 30));
	button_back.setText("Back");
	button_back.setCallback(cb);

	slider.setShape(Rect(15, 80, 210, 30));
	slider.setCallback(cbs);
	slider.setSliderPosition(50);

	m_container.pack(&button_back);
	m_container.pack(&slider);

	m_series1.fill(imu.get_data().gx);
	m_series2.fill(imu.get_data().gy);
	m_series3.fill(imu.get_data().gz);

	//m_series1.fill(s_throttle.value);
	//m_series2.fill(s_roll.value);
	//m_series3.fill(s_pitch.value);
	m_series4.fill(s_yaw.value);

	m_series1.setRange(-500, 500);
	m_series2.setRange(-500, 500);
	m_series3.setRange(-500, 500);

	//m_series1.setRange(Stick::min, Stick::max);
	//m_series2.setRange(Stick::min, Stick::max);
	//m_series3.setRange(Stick::min, Stick::max);
	m_series4.setRange(Stick::min, Stick::max);

	m_graph.pack(&m_series1);
	m_graph.pack(&m_series2);
	m_graph.pack(&m_series3);
	m_graph.pack(&m_series4);

	m_series2.m_color = 0x07E0;
	m_series3.m_color = 0x001F;
	m_series4.m_color = 0xFFE0;
	
	Serial.println("IMUConfigState succesfully Initialised");
}

void IMUConfigState::handleTouch(const Touch& touch)
{
	m_container.handleTouch(touch);
}

void IMUConfigState::update()
{
	m_container.draw(m_force_redraw);

	if (micros() - m_graph_last_draw_time > 1e6 / m_graph_draw_rate)
	{
		m_graph_last_draw_time = micros();

		m_graph.clear();

		if (radio_has_connection())
		{
			m_series1.push(ackPayload.roll);
			m_series2.push(ackPayload.pitch);
			m_series3.push(ackPayload.yaw);
		}
		else
		{
			m_series1.push(imu.get_data().gx);
			m_series2.push(imu.get_data().gy);
			m_series3.push(imu.get_data().gz);
		}

		//m_series1.push(s_throttle.value);
		//m_series2.push(s_roll.value);
		//m_series3.push(s_pitch.value);
		m_series4.push(s_yaw.value);

		m_graph.draw();
	}

	m_force_redraw = false;
}