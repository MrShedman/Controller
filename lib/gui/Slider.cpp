
#include <Arduino.h>
#include "Slider.h"
#include "LCD.h"
#include "TextGFX.h"

Slider::Slider()
	:
	isPressed(false),
	percent(0),
	prev_offset(0),
	c_outline(0xDEF6),
	m_border(2),
	m_handle(),
	m_callback(nullptr)
{}

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
		if (t.event == Touch::Event::pressed && !isPressed)
		{
			isPressed = true;
			m_should_draw = true;
		}
	}

	if (t.event == Touch::Event::released && isPressed)
	{
		m_should_draw = true;
		isPressed = false;
	}

	if (isPressed)
	{
		percent = map((float)t.point.x, m_shape.x + m_border, m_shape.x + m_shape.w - m_border, 0.0f, 1.0f);

		percent = constrain(percent, 0.0f, 1.0f);

		m_should_draw = true;

		if (m_callback) m_callback(percent);
	}

	return isPressed;
}

void Slider::moveHandle(float percent)
{
	m_handle.x = map(percent, 0.0f, 1.0f, m_shape.x + m_border, m_shape.x + m_shape.w - m_handle.w - m_border);
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

		Rect temp0(m_shape);
		temp0.y += 10;
		temp0.h -= 20;
		temp0.w = m_handle.x - temp0.x + 12;

		Rect temp1(m_shape);
		temp1.x = m_handle.x + 12;
		temp1.y += 10;
		temp1.h -= 20;
		temp1.w = m_shape.w - temp1.x + 12 + 3;

		display.fillRoundRect(temp0, temp0.h / 2, m_normal_colour);
		display.fillRoundRect(temp1, temp1.h / 2, c_outline);

		if (m_text.length() > 0)
		{
			textgfx.setCursor(m_shape.x + 8, m_shape.y + m_shape.h / 2 - 7);
			textgfx.setTextSize(2);
			textgfx.print(m_text);
		}

		drawHandle(isPressed ? m_pressed_colour : m_normal_colour);

		display.fillCircle(m_handle.x + 12, m_handle.y - m_border + m_shape.h / 2, 7, BLACK);

		m_should_draw = false;
	}
}
