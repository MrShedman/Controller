
#pragma once

#include "Arduino.h"
#include "Component.h"

class TextBox : public Component
{
public:

	typedef void(*Callback)(float);

	TextBox();

	void setCallback(Callback c)
	{
		m_callback = c;
	}

	bool handleTouch(const Touch& t) override;

	void draw(bool force_draw = false) override;

private:

	Callback m_callback;

	bool m_isPressed;
	bool m_isTyping;
};