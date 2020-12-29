#pragma once

#include <Arduino.h>
#include "RingBuffer.h"
#include "Rect.h"
#include "LCD.h"
#include "System.h"

class Series
{
public:

	Series() : m_color(0xF800) {}

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

	void draw(const Rect& bounds, const uint16_t color)
	{
		const float sx = (float)(bounds.w-1) / (float)(m_fifo_length-2);
		//const float sy = (float)bounds.h / (m_max - m_min);

		const Element* h = m_fifo.tail();
		const Element* prev_h = m_fifo.tail();
		
		int16_t x0 = bounds.x;
		int16_t y0 = mapf(prev_h->value, m_min, m_max, bounds.y + bounds.h, bounds.y);

		int16_t x1;
		int16_t y1;

		for (uint8_t i = 0; i < m_fifo.size()-2; ++i)
		{
			h = m_fifo.next(h);

			x1 = (float)(i + 1) * sx + (float)bounds.x;

			y1 = mapf(h->value, m_min, m_max, bounds.y + bounds.h, bounds.y);

			display.drawLine(x0, y0, x1, y1, color);

			prev_h = h;

			x0 = x1;
			y0 = y1;
		}
	}

	void setRange(float min, float max)
	{
		m_max = max;
		m_min = min;
	}

	void setColor(uint16_t color)
	{
		m_color = color;
	}

	uint16_t getColor() const
	{
		return m_color;
	}

private:

	uint16_t m_color;

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

	float m_max;
	float m_min;

	const static uint16_t m_fifo_length = 64;

	RingBuffer<Element, m_fifo_length> m_fifo;
};