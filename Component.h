#pragma once

#include "Arduino.h"
#include "Rect.h"
#include "Touch.h"
#include "WString.h"

class Component
{
public:

	Component()
		:
		m_should_draw(true),
		m_shape(),
		m_normal_colour(0x1F1F),
		m_pressed_colour(0x07E0),
		m_priority(255)
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

	virtual void setText(const String& text)
	{
		m_text = text;
		m_should_draw = true;
	}

	const String& getText() const
	{
		return m_text;
	}

	void setPriority(uint8_t p)
	{
		m_priority = p;
	}

	uint8_t getPriority() const
	{
		return m_priority;
	}

protected:

	uint8_t m_priority;

	uint16_t m_normal_colour;
	uint16_t m_pressed_colour;

	bool m_should_draw;
	Rect m_shape;
	String m_text;
};