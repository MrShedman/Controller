#pragma once

#include <Arduino.h>
#include "Series.h"
#include "LCD.h"

extern LCD display;

template <uint8_t NUM>
class Graph
{
public:

	Graph()
		:
		m_bounds(15, 115, 210, 154),
		index(0)
	{}

	void pack(Series* series)
	{
		if (index >= NUM) return;

		m_series[index++] = series;
	}

	void setSize(const Rect& r)
	{
		m_bounds = r;
	}

	uint8_t numSeries() const
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
			m_series[i]->draw(m_bounds, m_series[i]->getColor());
		}
	}

private:

	Rect m_bounds;

	uint8_t index;
	Series* m_series[NUM];
};