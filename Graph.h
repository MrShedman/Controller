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
		m_last_draw_time(0)
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

	void draw()
	{
		//if (micros() - m_last_draw_time > 1e6 / 60)
		//if (millis() - m_last_draw_time > 1000 / 60)
		{
			m_last_draw_time = micros();

			display.fillRect(Rect(0, 30, 240, 250), BLACK);

			for (uint8_t i = 0; i < index; ++i)
			{
				m_series[i]->draw(Rect(15, 40, 210, 240));
			}
		}
	}

private:

	uint32_t m_last_draw_time;
	uint8_t index;
	const uint8_t m_size;
	Series* m_series[NUM];
};