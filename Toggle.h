
#pragma once

#include "Arduino.h"
#include "Component.h"

class Toggle : public Component
{
public:

	typedef void (*Callback)(bool);

	Toggle();

	void setCallback(Callback c)
	{
		m_callback = c;
	}
		
	bool handleTouch(const Touch& t) override;
	
	void draw(bool force_draw = false) override;

private:

	Callback m_callback;

	bool isPressed, state;
};