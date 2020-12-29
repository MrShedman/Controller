
#ifndef _POINT_H_
#define _POINT_H_

#include <Arduino.h>

class Point
{
public:

	Point()
		:
		x(0),
		y(0)
	{}

	Point(int16_t x, int16_t y)
		:
		x(x),
		y(y)
	{}
	
	Point(const Point& rhs)
		:
		x(rhs.x),
		y(rhs.y)
	{}
	
	bool operator==(const Point& p1) 
	{
		return ((p1.x == x) && (p1.y == y));
	}

	bool operator!=(const Point& p1) 
	{
		return !operator==(p1);
	}

public:

	int16_t x, y;
};

#endif
