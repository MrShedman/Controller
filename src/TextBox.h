
#pragma once

#include <Arduino.h>
#include "Component.h"
#include "WString.h"

class TextBox : public Component
{
public:

	typedef void(*Callback)(void);

	TextBox();

	void setCallback(Callback c)
	{
		m_callback = c;
	}

	void externalCallback(const String& text)
	{
		m_number = text.toFloat();
		
		m_text = String(m_number);

		setChild(nullptr);

		m_isTyping = false;
		m_callback();
	}

	bool handleTouch(const Touch& t) override;

	void draw(bool force_draw = false) override;

private:

	float m_number;

	bool m_isPressed;
	bool m_isTyping;

	Callback m_callback;
};