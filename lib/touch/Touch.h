#pragma once

#include <Arduino.h>

#include "Point.h"

class Touch 
{
public:

	enum Event : uint8_t
	{
		pressed = 0x00,
		released = 0x01,
		moved = 0x02,
		none = 0x03
	};

	Touch()
		:
		point(),
		event(none),
		time(0)
	{}

	Touch(uint16_t x0, uint16_t y0)
		:
		point(x0, y0),
		event(none),
		time(0)
	{}

	bool operator==(const Touch& t1) 
	{
		return (point == t1.point) && (event == t1.event);
	}

	bool operator!=(const Touch& t1) 
	{
		return !operator==(t1);
	}

	Point point;
	Event event;
	uint32_t time; //millis
};