
#include "Arduino.h"
#include "Toggle.h"
#include "LCD.h"
#include "TextGFX.h"

extern LCD display;
extern TextGFX textgfx;

Toggle::Toggle()
	:
	isPressed(false),
	state(false),
	m_callback(nullptr)
{

}

void Toggle::handleTouch(const Touch& t)
{
	if (!m_shape.contains(t.point))
	{
		if (isPressed)
		{
			m_should_draw = true;
		}

		isPressed = false;
		return;
	}

	if (t.event == Touch::pressed)
	{
		isPressed = true;
		m_should_draw = true;
	}

	if (t.event == Touch::released && isPressed)
	{
		state = !state;
		m_should_draw = true;
		if (m_callback) m_callback(state);
	}
}


void Toggle::draw(bool force_draw)
{
	if (m_should_draw || force_draw)
	{
		display.fillRoundRect(m_shape, 8, state ? m_pressed_colour : m_normal_colour);

		textgfx.setCursor(m_shape.x + 8, m_shape.y + m_shape.h / 2 - 7);
		textgfx.setTextSize(2);
		textgfx.print(m_text);

		m_should_draw = false;
	}
}