#pragma once

#include "TextGFX.h"
#include "LCD.h"

extern TextGFX textgfx;

template <class T>
class Drawable
{
public:

	Drawable()
		:
		cx(-1),
		cy(-1),
		value(nullptr)
	{

	}

	void draw(T v)
	{
		priv_draw(*value, BLACK);
		*value = &v;
		priv_draw(v, WHITE);
	}

	void draw(bool force_draw = false)
	{
		bool has_changed = false;

		if (value && *value != prev_value)
		{
			has_changed = true;
			priv_draw(prev_value, BLACK);
		}

		if (force_draw || has_changed)
		{
			priv_draw(*value, WHITE);
		}

		if (has_changed)
		{
			prev_value = *value;
		}
	}

	void setValue(T* v)
	{
		value = v;
		prev_value = *value;
	}

	void setPosition(int16_t x, int16_t y)
	{
		cx = x;
		cy = y;
	}

private:

	void priv_draw(T value, uint16_t color)
	{
		if (cx > 0 || cy > 0)
		{
			textgfx.setCursor(cx, cy);
		}

		textgfx.setTextColor(color);
		textgfx.print(value);
		textgfx.setTextColor(WHITE);
	}

	int16_t cx, cy;

	T* value;
	T prev_value;
};