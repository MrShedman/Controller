#pragma once

#include <Arduino.h>
#include "Point.h"

class Touch 
{
public:

	enum class Event : uint8_t
	{
		pressed = 0x00,
		released = 0x01,
		moved = 0x02,
		none = 0x03
	};

	enum class Gesture : uint8_t
	{
		swipe_up,
		swipe_down,
		swipe_left,
		swipe_right,
		none
	};

	Touch()
		:
		point(),
		event(Event::none),
		gesture(Gesture::none),
		gestureVel(0.0f),
		time(0)
	{}

	Touch(uint16_t x0, uint16_t y0)
		:
		point(x0, y0),
		event(Event::none),
		gesture(Gesture::none),
		gestureVel(0.0f),
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
	Gesture gesture;
	float gestureVel;
	uint32_t time; //millis
};