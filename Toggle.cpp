
#include "Arduino.h"
#include "Toggle.h"
#include "LCD.h"
#include "TextGFX.h"
#include "GUIConstants.h"

Toggle::Toggle()
	:
	isPressed(false),
	state(false),
	m_callback(nullptr)
{
	setPriority(GUIPriority::p_Toggle);
}

bool Toggle::handleTouch(const Touch& t)
{
	bool used_event = false;

	if (!m_shape.contains(t.point))
	{
		if (isPressed)
		{
			m_should_draw = true;
		}

		isPressed = false;
	}
	else
	{

		if (t.event == Touch::pressed)
		{
			isPressed = true;
			m_should_draw = true;
			used_event = true;
		}

		if (t.event == Touch::released && isPressed)
		{
			state = !state;
			m_should_draw = true;
			used_event = true;
			if (m_callback) m_callback(state);
		}
	}

	return used_event;
}


void Toggle::draw(bool force_draw)
{
	if (m_should_draw || force_draw)
	{
		display.fillRoundRect(m_shape, 8, state ? m_pressed_colour : m_normal_colour);

		if (m_text.length() > 0)
		{
			textgfx.setCursor(m_shape.x + 8, m_shape.y + m_shape.h / 2 - 7);
			textgfx.setTextSize(2);
			textgfx.print(m_text);
		}

		m_should_draw = false;
	}
}