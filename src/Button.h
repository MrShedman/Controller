
#pragma once

#include "Arduino.h"
#include "Component.h"

class Button : public Component
{
public:

	typedef void (*Callback)(void);

	Button();

	void setCallback(Callback c)
	{
		m_callback = c;
	}
		
	virtual bool handleTouch(const Touch& t) override;
	
	virtual void draw(bool force_draw = false) override;

	void enableMultiPress(bool flag, uint32_t delay)
	{
		m_multi_press = flag;
		m_multi_press_delay = delay;
	}

protected:

	bool m_isPressed;

	uint32_t m_time_since_press;
	uint32_t m_multi_press_delay;
	bool m_multi_press;

	Callback m_callback;
};