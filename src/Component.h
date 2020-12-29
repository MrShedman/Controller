#pragma once

#include <Arduino.h>
#include "Rect.h"
#include "Touch.h"
#include "WString.h"

class Component
{
public:

	Component()
		:
		m_child(nullptr),
		m_parent(nullptr),
		m_normal_colour(0x1F1F),
		m_pressed_colour(0x07E0),
		m_should_draw(true),
		m_shape(),
		m_text()
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

	Point getSize() const
	{
		return Point(m_shape.w, m_shape.h);
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

	void setParent(Component* parent)
	{
		if (parent)
		{
			m_parent = parent;
			m_parent->m_child = this;
		}
		else
		{
			if (m_parent)
			{
				m_parent->m_child = nullptr;
				m_parent = nullptr;
			}
		}
	}

	void setChild(Component* child)
	{
		if (child)
		{
			m_child = child;
			m_child->m_parent = this;
		}
		else
		{
			if (m_child)
			{
				m_child->m_parent = nullptr;
				m_child = nullptr;
			}
		}
	}

	uint8_t getChildLevel() const
	{
		if (m_child != nullptr)
		{
			return m_child->getChildLevel() + 1u;
		}
		else
		{
			return 1u;
		}
	}

protected:

	Component* m_child;
	Component* m_parent;

	uint16_t m_normal_colour;
	uint16_t m_pressed_colour;

	bool m_should_draw;
	Rect m_shape;
	String m_text;
};