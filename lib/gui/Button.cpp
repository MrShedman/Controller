
#include "Arduino.h"
#include "Button.h"
#include "LCD.h"
#include "TextGFX.h"

Button::Button()
	:
	m_isPressed(false),
	m_time_since_press(0),
	m_multi_press_delay(0),
	m_multi_press(false),
	m_callback(nullptr)
{}

bool Button::handleTouch(const Touch& t)
{
	bool used_event = false;

	if (!m_shape.contains(t.point))
	{
		if (m_isPressed)
		{
			m_should_draw = true;
		}

		m_isPressed = false;
	}
	else
	{
		if (t.event == Touch::pressed && !m_isPressed)
		{
			m_isPressed = true;
			m_should_draw = true;
			m_time_since_press = millis();

			used_event = true;
		}

		if (t.event == Touch::moved && m_multi_press && m_isPressed)
		{
			if ((millis() - m_time_since_press) > m_multi_press_delay)
			{
				m_time_since_press = millis();
				if (m_callback) m_callback();
			}

			used_event = true;
		}

		if (t.event == Touch::released && m_isPressed)
		{
			m_isPressed = false;
			m_should_draw = true;
			used_event = true;

			if (m_callback) m_callback();
		}
	}

	return used_event;
}

void Button::draw(bool force_draw)
{
	if (m_should_draw || force_draw)
	{
		display.fillRoundRect(m_shape, 8, m_isPressed ? m_pressed_colour : m_normal_colour);
		
		if (m_text.length() > 0)
		{
			textgfx.setCursor(m_shape.x + 8, m_shape.y + m_shape.h / 2 - 7);
			textgfx.setTextSize(2);
			textgfx.print(m_text);
		}

		m_should_draw = false;
	}
}
