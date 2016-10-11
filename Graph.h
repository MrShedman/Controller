#pragma once

#include "Arduino.h"
#include "Series.h"
#include "LCD.h"

extern LCD display;

template <uint8_t NUM>
class Graph
{
public:

	Graph()
		:
		m_size(NUM),
		index(0),
		m_bounds(15, 115, 209, 154)
	{

	}

	void pack(Series* series)
	{
		if (index >= m_size) return;

		m_series[index++] = series;
	}

	uint8_t size() const
	{
		return index;
	}

	const Series* operator[](int index) const
	{
		return m_series[index];
	}

	Series* operator[](int index)
	{
		return m_series[index];
	}

	void clear()
	{
		for (uint8_t i = 0; i < index; ++i)
		{
			m_series[i]->draw(m_bounds, BLACK);
		}
	}

	void draw()
	{
		for (uint8_t i = 0; i < index; ++i)
		{
			m_series[i]->draw(m_bounds, m_series[i]->m_color);
		}
	}

private:

	Rect m_bounds;

	uint8_t index;
	const uint8_t m_size;
	Series* m_series[NUM];
};