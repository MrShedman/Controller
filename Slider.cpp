
#include "Arduino.h"
#include "Slider.h"
#include "LCD.h"
#include "TextGFX.h"
#include "GUIConstants.h"
#include "System.h"

Slider::Slider()
	:
	isPressed(false),
	percent(0),
	prev_offset(0),
	m_border(2),
	c_outline(0xDEF6),
	m_callback(nullptr)
{
	setPriority(GUIPriority::p_Slider);
}

void Slider::setShape(Rect shape)
{
	m_shape = shape;
	m_handle = Rect(shape.x + m_border, shape.y + m_border, shape.h - 2 * m_border, shape.h - 2 * m_border);
		
	m_should_draw = true;
}

void Slider::setSize(Point size)
{
	m_shape.w = size.x;
	m_shape.h = size.y;

	m_handle.w = m_shape.h - 2 * m_border;
	m_handle.h = m_handle.w;

	m_should_draw = true;
}

bool Slider::handleTouch(const Touch& t)
{
	if (m_shape.contains(t.point))
	{
		if (t.event == Touch::pressed && !isPressed)
		{
			isPressed = true;
			m_should_draw = true;
		}
	}

	if (t.event == Touch::released && isPressed)
	{
		m_should_draw = true;
		isPressed = false;
	}

	if (isPressed)
	{
		percent = mapf(t.point.x, m_shape.x + m_border, m_shape.x + m_shape.w - m_border, 0.0f, 1.0f);

		percent = constrain(percent, 0.0f, 1.0f);

		m_should_draw = true;

		if (m_callback) m_callback(percent);
	}

	return isPressed;
}

void Slider::moveHandle(float percent)
{
	m_handle.x = mapf(percent, 0.0f, 1.0f, m_shape.x + m_border, m_shape.x + m_shape.w - m_handle.w - m_border);
}

void Slider::drawHandle(uint16_t color) const
{
	const uint8_t rad = 12;

	display.fillCircle(m_handle.x + rad, m_handle.y - m_border + m_shape.h / 2, rad, color);
}

void Slider::draw(bool force_draw)
{
	if (m_should_draw || force_draw)
	{
		drawHandle(BLACK);

		moveHandle(percent);

		Rect temp(m_shape);
		temp.y += 10;
		temp.h -= 20;

		display.fillRoundRect(temp, temp.h/2, c_outline);

		if (m_text.length() > 0)
		{
			textgfx.setCursor(m_shape.x + 8, m_shape.y + m_shape.h / 2 - 7);
			textgfx.setTextSize(2);
			textgfx.print(m_text);
		}

		drawHandle(isPressed ? m_pressed_colour : m_normal_colour);

		m_should_draw = false;
	}
}
