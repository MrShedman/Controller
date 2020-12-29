#pragma once

#include "Toggle.h"
#include "Icons.h"

class CheckBox : public Toggle
{
public:

	CheckBox()
		:
		Toggle()
	{
		m_boxShape = getBitmapRect(checkbox_unchecked);
	}

	void set()
	{
		state = true;
		m_should_draw = true;
	}

	void clear()
	{
		state = false;
		m_should_draw = true;
	}

	void draw(bool force_draw = false) override;

private:

	Rect m_boxShape;
};