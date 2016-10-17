
#include "Arduino.h"
#include "Slider.h"
#include "LCD.h"
#include "TextGFX.h"

extern LCD display;
extern TextGFX textgfx;

Slider::Slider()
	:
	isPressed(false),
	percent(0),
	prev_offset(0),
	border(2),
	c_outline(0xDEF6),
	m_callback(nullptr)
{

}

void Slider::setShape(Rect shape)
{
	m_shape = shape;
	slider = Rect(shape.x + border, shape.y + border, shape.h - 2 * border, shape.h - 2 * border);

	m_should_draw = true;
}

void Slider::setSize(Point size)
{
	m_shape.w = size.x;
	m_shape.h = size.y;

	slider.w = m_shape.h - 2 * border;
	slider.h = slider.w;

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
		uint16_t p = constrain(t.point.x, m_shape.x + border, m_shape.x + m_shape.w - border);

		percent = map(p, m_shape.x + border, m_shape.x + m_shape.w - border, 0, 65535);

		m_should_draw = true;

		if (m_callback) m_callback(percent / 655);
	}

	return isPressed;
}

void Slider::draw(bool force_draw)
{
	if (m_should_draw || force_draw)
	{
		const uint8_t diam = 12;

		display.fillCircle(slider.translate(prev_offset, 0).x + diam, slider.y - border + m_shape.h / 2, diam, BLACK);

		uint16_t offset = map(percent, 0, 65535, 0, m_shape.w - slider.w - 2 * border);

		prev_offset = offset;

		//display.fillRoundRect(m_shape, 8, c_outline);

		Rect temp(m_shape);
		temp.y += 10;
		temp.h -= 20;

		display.fillRoundRect(temp, temp.h/2, c_outline);

		if (m_text)
		{
			textgfx.setCursor(m_shape.x + 8, m_shape.y + m_shape.h / 2 - 7);
			textgfx.setTextSize(2);
			textgfx.print(m_text);
		}

		//display.fillRoundRect(slider.translate(offset, 0), 8, isPressed ? m_pressed_colour : m_normal_colour);

		display.fillCircle(slider.translate(offset,0).x + diam, slider.y - border + m_shape.h/2, diam, isPressed ? m_pressed_colour : m_normal_colour);

		m_should_draw = false;
	}
}
