
#pragma once

#include "Arduino.h"
#include "Component.h"

class Slider : public Component
{
public:

	typedef void (*Callback)(float);

	Slider();

	void setCallback(Callback c)
	{
		m_callback = c;
	}
		
	bool handleTouch(const Touch& t) override;
	
	void draw(bool force_draw = false) override;

	void setShape(Rect shape) override;

	void setSize(Point size) override; 

	void setSliderPosition(float p)
	{
		p = constrain(p, 0.0f, 1.0f);
		percent = p;
		m_should_draw = true;
	}

private:

	void moveHandle(float percent);

	void drawHandle(uint16_t color) const;

	Callback m_callback;
	
	float percent;

	uint16_t prev_offset;
		
	uint16_t c_outline;

	uint8_t m_border;

	Rect m_handle;

	bool isPressed;
};