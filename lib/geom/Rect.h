#pragma once

#include <Arduino.h>

#include "Point.h"

class Rect
{
public:
	Rect()
		:
		x(0),
		y(0),
		w(0),
		h(0)
	{}

	Rect(int16_t x, int16_t y, int16_t w, int16_t h)
		:
		x(x),
		y(y),
		w(w),
		h(h)
	{}

	Rect(const Rect &rhs)
		:
		x(rhs.x),
		y(rhs.y),
		w(rhs.w),
		h(rhs.h)
	{}
	
	bool contains(int16_t x1, int16_t y1) const
	{
		return (x1 >= x && x1 < (x + w) && y1 < (y + h) && y1 >= y);
	}
	
	bool contains(const Point& p) const
	{
		return contains(p.x, p.y);
	}

	int32_t area() const
	{
		return (int32_t)w * (int32_t)h;
	}

	Rect& move(int16_t dx, int16_t dy)
	{
		x += dx;
		y += dy;
		return *this;
	}

	Rect& inflate(int16_t sx, int16_t sy)
	{
		x += sx;
		w -= 2 * sx;
		y += sy;
		h -= 2 * sy;

		return *this;
	}

	Rect translate(int16_t dx, int16_t dy) const
	{
		Rect t(*this);
		t.x += dx;
		t.y += dy;
		return t;
	}
	
	Rect getClip(const Rect& rhs) const
	{
		Rect clip;
		clip.x = max(x, rhs.x);
		clip.y = max(y, rhs.y);
		clip.w = min(x + w, rhs.x + rhs.w) - clip.x;
		clip.h = min(y + h, rhs.y + rhs.h) - clip.y;
		return clip;
	}
	
	Point getCenter() const
	{
		int16_t midX = x + w / 2;
		int16_t midY = y + h / 2;

		return Point(midX, midY);
	}

	inline bool operator==(const Rect& rhs) const
	{
		return x == rhs.x && y == rhs.y && w== rhs.w && h== rhs.h;
	}

	inline bool operator!=(const Rect& rhs) const
	{
		return !operator==(rhs);
	}

public:

	int16_t x, y, w, h;
};