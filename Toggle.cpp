
#include "Arduino.h"
#include "Toggle.h"
#include "LCD.h"
#include "TextGFX.h"

Toggle::Toggle()
	:
	isPressed(false),
	state(false),
	m_callback(nullptr)
{}

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

		if (t.event == Touch::Event::pressed)
		{
			isPressed = true;
			m_should_draw = true;
			used_event = true;
		}

		if (t.event == Touch::Event::released && isPressed)
		{
			state = !state;
			isPressed = false;
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