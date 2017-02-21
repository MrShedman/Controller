#pragma once

#include "CheckBox.h"
#include "Button.h"
#include "Component.h"

class OptionButton : public Component
{
public:

	typedef void(*Callback)(uint8_t);

	enum CallbackType
	{
		Enter,
		Back,
		OK
	};

	OptionButton();

	void set(uint8_t index)
	{
		if (index < m_size)
		{
			clear();

			m_checkboxes[index].set();

			m_enter_button.setText(m_checkboxes[index].getText());

			m_should_draw = true;
		}
	}

	void setShape(Rect shape) override
	{
		m_enter_button.setShape(shape);
	
		m_should_draw = true;
	}

	void setSize(Point size) override
	{
		m_enter_button.setSize(size);

		m_should_draw = true;
	}

	void addOption(const String& text)
	{
		if (m_size < n)
		{
			m_checkboxes[m_size++].setText(text);

			updateShapes();
		}
	}

	void setCallback(Callback c)
	{
		m_callback = c;
	}

	void externalCallback(CallbackType c);

	bool handleTouch(const Touch& t) override;

	void draw(bool force_draw = false) override;

	static bool isOpen()
	{
		return m_in_selection;
	}

	static void close();

private:

	void updateShapes();

	void clear()
	{
		for (uint8_t i = 0; i < m_size; ++i)
		{
			m_checkboxes[i].clear();
		}
	}

	Callback m_callback;

	const static uint8_t n = 5;

	uint8_t m_size;

	static bool m_in_selection;

	uint8_t m_selected;

	Button m_enter_button;

	Button m_back_button;
	Button m_ok_button;

	CheckBox m_checkboxes[n];
};