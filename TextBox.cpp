
#include "Arduino.h"
#include "TextBox.h"
#include "LCD.h"
#include "TextGFX.h"
#include "Numpad.h"

namespace
{
	TextBox* current_active = nullptr;

	void texbox_callback(const String& text)
	{
		if (current_active)
		{
			current_active->externalCallback(text);
			current_active = nullptr;
		}
	}
}

TextBox::TextBox()
	:
	m_number(0.0f),
	m_isPressed(false),
	m_isTyping(false),
	m_callback(nullptr)
{
	m_text = "0.00";
}

bool TextBox::handleTouch(const Touch& t)
{
	if (numpad.isOpen())
	{
		if (m_isTyping)
		{
			return numpad.handleTouch(t);
		}
		else
		{
			return false;
		}
	}

	bool used_event = false;

	if (!m_shape.contains(t.point))
	{
		if (m_isPressed)
		{
			m_should_draw = true;
		}

		m_isPressed = false;
		return false;
	}

	if (t.event == Touch::Event::pressed && !m_isPressed)
	{
		m_isPressed = true;
		m_should_draw = true;

		used_event = true;
	}

	if (t.event == Touch::Event::released && m_isPressed)
	{
		m_isPressed = false;
		m_isTyping = true;
		m_should_draw = true;
		
		current_active = this;
		numpad.setCallback(texbox_callback);
		numpad.open();
		numpad.setText(m_text);
		
		setChild(&numpad);

		display.fillRect(Rect(0, 30, 240, 250), BLACK);

		used_event = true;
	}

	return used_event;
}

void TextBox::draw(bool force_draw)
{
	if (m_isTyping && numpad.isOpen())
	{
		numpad.draw(m_should_draw || force_draw);
		m_should_draw = false;
		return;
	}

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
