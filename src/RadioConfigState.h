#pragma once

#include "State.h"

#include "Slider.h"
#include "Container.h"
#include "TextBox.h"
#include "CheckBox.h"
#include "OptionButton.h"

class RadioConfigState : public State
{
public:

	void setup() override;

	void handleTouch(const Touch& touch) override;

	void update() override;

private:

	Container<6> m_container;

	CheckBox checkbox;

	OptionButton optionbutton;

	Slider slider_channel;
	Slider slider_data_rate;

	TextBox textbox;
	TextBox textbox1;
};