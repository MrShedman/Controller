#pragma once

#include "State.h"

#include "Button.h"
#include "Toggle.h"
#include "Slider.h"
#include "Container.h"

class RadioConfigState : public State
{
public:

	void setup() override;

	void handleTouch(const Touch& touch) override;

	void update() override;

private:

	Container<6> m_container;

	Button button_back;

	Toggle toggle_ACK_payloads;

	Slider slider_channel;
	Slider slider_PA_level;
	Slider slider_data_rate;
	Slider slider_refresh_rate;
};