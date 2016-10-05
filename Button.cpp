
#include "Arduino.h"
#include "Button.h"
#include "LCD.h"
#include "TextGFX.h"

extern LCD display;
extern TextGFX textgfx;

Button::Button()
	:
	m_isPressed(false),
	m_time_since_press(0),
	m_multi_press_delay(0),
	m_multi_press(false),
	m_callback(nullptr)
{
}

void Button::handleTouch(const Touch& t)
{
	if (!m_shape.contains(t.point))
	{
		if (m_isPressed)
		{
			m_should_draw = true;
		}

		m_isPressed = false;
		return;
	}

	if (t.event == Touch::pressed && !m_isPressed)
	{
		m_isPressed = true;
		m_should_draw = true;
		m_time_since_press = millis();
	}

	if (t.event == Touch::moved && m_multi_press)
	{
		if ((millis() - m_time_since_press) > m_multi_press_delay && m_isPressed)
		{
			m_time_since_press = millis();
			if (m_callback) m_callback();
		}
	}

	if (t.event == Touch::released && m_isPressed)
	{
		m_isPressed = false;
		m_should_draw = true;
		if (m_callback) m_callback();
	}
}

void Button::draw(bool force_draw)
{
	if (m_should_draw || force_draw)
	{
		display.fillRoundRect(m_shape, 8, m_isPressed ? m_pressed_colour : m_normal_colour);
		
		textgfx.setCursor(m_shape.x + 8, m_shape.y + m_shape.h / 2 - 7);
		textgfx.setTextSize(2);
		textgfx.print(m_text);
		
		m_should_draw = false;
	}
}
