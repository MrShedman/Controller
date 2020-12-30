#pragma once

#include <Arduino.h>
#include "circular_buffer.h"
#include "Rect.h"
#include "Point.h"
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
		const float sx = (float)(bounds.w-1) / (float)(m_fifo_length);

		for (uint8_t i = 0; i < m_fifo.size(); ++i)
		{
			m_fifo[i].point.x = (float)(i) * sx + (float)bounds.x;
			m_fifo[i].point.y = map(m_fifo[i].value, m_min, m_max, bounds.y + bounds.h, bounds.y);
			m_fifo[i].point.y = constrain(m_fifo[i].point.y, bounds.y, bounds.y + bounds.h);
		}

		for (uint8_t i = 0; i < m_fifo.size() - 1; ++i)
		{
			display.drawLine(m_fifo[i].point.x, m_fifo[i].point.y, m_fifo[i+1].point.x, m_fifo[i+1].point.y, color);
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
			time(0u),
			point()
		{}

		Element(float value, uint32_t time)
			:
			value(value),
			time(time),
			point()
		{}

		float value;
		uint32_t time;
		Point point;
	};

	float m_max;
	float m_min;

	const static uint16_t m_fifo_length = 64;

	CircularBuffer<Element, m_fifo_length> m_fifo;
};