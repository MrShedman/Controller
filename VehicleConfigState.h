#pragma once

#include "State.h"

#include "Button.h"
#include "Toggle.h"
#include "Slider.h"
#include "Container.h"

class VehicleConfigState : public State
{
public:

	void setup() override;

	void handleTouch(const Touch& touch) override;

	void update() override;

private:

	Container<1> m_container;

	Button button_back;
};
