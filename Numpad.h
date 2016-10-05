
#pragma once

#include "Arduino.h"
#include "Component.h"
#include "Container.h"
#include "Button.h"

class Numpad : public Component
{
public:

	typedef void(*Callback)(float);

	Numpad();

	void setCallback(Callback c)
	{
		m_callback = c;
	}

	void externalCallback(float val)
	{
		if (m_callback != nullptr)
		{
			m_callback(val);
		}
	}

	void handleTouch(const Touch& t) override;

	void draw(bool force_draw = false) override;

private:

	const uint8_t px = 15;
	const uint8_t py = 100;

	const uint8_t dx = 6;
	const uint8_t dy = 6;
	const uint8_t w = 48;
	const uint8_t h = 36;

	Container<13> m_container;

	Button button_enter;
	Button button_delete;
	Button button_decimal;
	Button button_numbers[10];

	Callback m_callback;
};

extern Numpad numpad;