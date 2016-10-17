
#pragma once

#include "Arduino.h"
#include "Component.h"

class Slider : public Component
{
public:

	typedef void (*Callback)(int);

	Slider();

	void setCallback(Callback c)
	{
		m_callback = c;
	}
		
	bool handleTouch(const Touch& t) override;
	
	void draw(bool force_draw = false) override;

	void setShape(Rect shape) override;

	void setSize(Point size) override; 

	void setSliderPosition(uint8_t p)
	{
		percent = map(p, 0, 100, 0, 65535);
		m_should_draw = true;
	}

private:

	Callback m_callback;
	
	uint16_t percent;

	uint16_t prev_offset;
	
	uint8_t border;
	
	uint16_t c_outline;

	Rect slider;
	
	bool isPressed;
};