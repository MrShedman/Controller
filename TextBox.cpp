
#include "Arduino.h"
#include "TextBox.h"
#include "LCD.h"
#include "TextGFX.h"
#include "Numpad.h"

TextBox::TextBox()
	:
	m_isPressed(false),
	m_isTyping(false),
	m_callback(nullptr)
{
}

void TextBox::handleTouch(const Touch& t)
{
	if (m_isTyping)
	{
		numpad.handleTouch(t);
		return;
	}

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
	}

	if (t.event == Touch::released && m_isPressed)
	{
		m_isPressed = false;
		m_isTyping = true;
		m_should_draw = true;
		//if (m_callback) m_callback();
	}
}

void TextBox::draw(bool force_draw)
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
