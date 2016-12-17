
#pragma once

#include "Arduino.h"
#include "Component.h"
#include "Container.h"
#include "Button.h"

class Numpad : public Component
{
public:

	typedef void(*Callback)(const String&);

	Numpad();

	void setCallback(Callback c)
	{
		m_callback = c;
	}

	bool handleTouch(const Touch& t) override;

	void draw(bool force_draw = false) override;

	void setText(const String& text) override;

	void open()
	{
		m_open = true;
	}

	void close()
	{
		m_open = false;

		if (m_callback != nullptr)
		{
			m_callback(m_text);
		}
	}

	bool isOpen() const
	{
		return m_open;
	}

private:

	bool m_open;

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