#pragma once

#include "Arduino.h"
#include "Rect.h"
#include "Touch.h"

class Component
{
public:

	Component()
		:
		m_should_draw(true),
		m_shape(),
		m_normal_colour(0xF800),
		m_pressed_colour(0x07E0)
	{}

	virtual ~Component(){}

	virtual bool handleTouch(const Touch& t) { return false; }

	virtual void draw(bool force_draw = false) {}

	virtual void setShape(Rect shape)
	{
		m_shape = shape;
		m_should_draw = true;
	}

	virtual void setSize(Point size)
	{
		m_shape.w = size.x;
		m_shape.h = size.y;
		m_should_draw = true;
	}

	void setPosition(Point p)
	{
		m_shape.x = p.x;
		m_shape.y = p.y;
		m_should_draw = true;
	}

	void shouldDraw(bool flag)
	{
		m_should_draw = flag;
	}

	void setText(const char* text)
	{
		m_text = text;
		m_should_draw = true;
	}

protected:

	uint16_t m_normal_colour;
	uint16_t m_pressed_colour;

	bool m_should_draw;
	Rect m_shape;
	const char* m_text;
};