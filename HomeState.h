#pragma once

#include "State.h"

#include "Button.h"
#include "Toggle.h"
#include "Slider.h"
#include "Container.h"

class HomeState : public State
{
public:

	void setup() override;

	void handleTouch(const Touch& touch) override;

	void update() override;

private:

	Container<3> m_container;

	Button but1;

	Button but2;

	Button but3;

	uint32_t loop_start_time;
};