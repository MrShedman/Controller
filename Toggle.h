
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

	bool isSelected() const
	{
		return state;
	}
		
	virtual bool handleTouch(const Touch& t) override;
	
	virtual void draw(bool force_draw = false) override;

protected:

	bool isPressed, state;

	Callback m_callback;
};