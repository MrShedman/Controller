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

	void draw(const Rect& bounds)
	{
		float sx = (float)bounds.w / (float)64;
		float sy = (float)bounds.h / 1000.0f;

		//float tx = 

		Element* h = m_fifo.head();
		Element* prev_h = m_fifo.head();

		float tb = millis() - prev_h->time;

		for (int i = 1; i < m_fifo.size(); ++i)
		{
			h = m_fifo.next(h);

			float y0 = map(prev_h->value, 1000, 2000, bounds.y, bounds.y + bounds.h);
			float y1 = map(h->value, 1000, 2000, bounds.y, bounds.y + bounds.h);

			//float x0 = prev_h->time

			display.drawLine(i*sx + bounds.x, y0, (i+1)*sx + bounds.x, y1, color);
			//display.drawLine(i*sx + bounds.x, y0 + 1, (i + 1)*sx + bounds.x, y1 + 1, color);

			prev_h = h;
		}
	}

	uint16_t color = 0xF800;

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

	RingBuffer<Element, 64> m_fifo;
};