#pragma once

#include "Arduino.h"
#include "RingBuffer.h"
#include "Rect.h"
#include "LCD.h"

extern LCD Display;

class Series
{
public:

	Series()
	{

	}

	void push(float value)
	{
		m_fifo.push(Element(value, millis()));
	}

	void fill(float value)
	{
		uint32_t t = millis();

		while (!m_fifo.full())
		{
			m_fifo.push(Element(value, t));
		}
	}

	void draw(const Rect& bounds, uint16_t color)
	{
		float sx = (float)bounds.w / (float)m_fifo_length;
		float sy = (float)bounds.h / float(m_max - m_min);

		const Element* h = m_fifo.tail();
		const Element* prev_h = m_fifo.tail();
		
		for (int i = 2; i < m_fifo.size(); ++i)
		{
			h = m_fifo.next(h);

			float y0 = map(prev_h->value, m_max, m_min, bounds.y + bounds.h, bounds.y);
			float y1 = map(h->value, m_max, m_min, bounds.y + bounds.h, bounds.y);

			display.drawLine(i*sx + bounds.x, y0, (i+1)*sx + bounds.x, y1, color);

			prev_h = h;
		}
	}

	void setRange(int16_t max, int16_t min)
	{
		m_max = max;
		m_min = min;
	}

	uint16_t m_color = 0xF800;

private:

	struct Element
	{
		Element()
			:
			value(0.0f),
			time(0u)
		{}

		Element(float value, uint32_t time)
			:
			value(value),
			time(time)
		{}

		float value;
		uint32_t time;
	};

	int16_t m_max;
	int16_t m_min;

	const static uint16_t m_fifo_length = 64;

	RingBuffer<Element, m_fifo_length> m_fifo;
};