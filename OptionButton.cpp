#include "OptionButton.h"
#include "LCD.h"
#include "TextGFX.h"

namespace
{
	OptionButton* current_active = nullptr;

	void enter_button_callback()
	{
		if (current_active)
		{
			current_active->externalCallback(OptionButton::Enter);
			current_active = nullptr;
		}
	}

	void back_button_callback()
	{
		if (current_active)
		{
			current_active->externalCallback(OptionButton::Back);
			current_active = nullptr;
		}
	}

	void ok_button_callback()
	{
		if (current_active)
		{
			current_active->externalCallback(OptionButton::OK);
			current_active = nullptr;
		}
	}

	bool temp_callback = false;
	uint8_t temp_selected = UINT8_MAX;

	void checkbox_callback(bool state)
	{
		temp_callback = true;
	}
}

bool OptionButton::m_in_selection = false;

OptionButton::OptionButton()
	:
	m_callback(nullptr),
	m_size(0),
	m_selected(0)
{
	updateShapes();

	set(0);

	m_enter_button.setCallback(enter_button_callback);

	m_back_button.setText("cancel");
	m_ok_button.setText("ok");

	m_back_button.setCallback(back_button_callback);
	m_ok_button.setCallback(ok_button_callback);

}

void OptionButton::updateShapes()
{
	int16_t y = 40;

	for (uint8_t i = 0; i < m_size; ++i)
	{
		m_checkboxes[i].setShape(Rect(15, y, 210, 30));
		m_checkboxes[i].setCallback(checkbox_callback);

		y += 40;
	}

	m_back_button.setShape(Rect(15, y, 97, 30));
	m_ok_button.setShape(Rect(128, y, 97, 30));

	m_should_draw = true;
}

void OptionButton::externalCallback(CallbackType c)
{
	if (c == Enter)
	{
		m_in_selection = true;
	}
	else
	{
		if (c == OK)
		{
			m_selected = temp_selected;
		}

		close();

		if (m_callback) m_callback(m_selected);
	}
}


void OptionButton::close()
{
	m_in_selection = false;

	if (current_active)
	{
		temp_selected = current_active->m_selected;

		current_active->setChild(nullptr);
		current_active->set(current_active->m_selected);

		current_active = nullptr;
	}
}

bool OptionButton::handleTouch(const Touch& t)
{
	bool used_event = false;

	current_active = this;

	if (!m_in_selection)
	{
		used_event = m_enter_button.handleTouch(t);

		if (m_in_selection)
		{
			setChild(&m_checkboxes[0]);
			m_should_draw = true;
			used_event = true;
			display.fillRect(Rect(0, 30, 240, 250), BLACK);
		}
	}
	else
	{
		if (m_back_button.handleTouch(t))
		{

		}
		else if (m_ok_button.handleTouch(t))
		{

		}
		else
		{
			for (uint8_t i = 0; i < m_size; ++i)
			{
				m_checkboxes[i].handleTouch(t);

				if (temp_callback)
				{
					temp_selected = i;

					set(temp_selected);

					temp_callback = false;
					break;
				}
			}
		}

		used_event = true;
	}

	return used_event;
}

void OptionButton::draw(bool force_draw)
{
	bool d = (m_should_draw || force_draw);

	if (!m_in_selection)
	{
		m_enter_button.draw(d);
	}
	else
	{
		for (uint8_t i = 0; i < m_size; ++i)
		{
			m_checkboxes[i].draw(d);
		}

		m_back_button.draw(d);
		m_ok_button.draw(d);
	}

	m_should_draw = false;
}