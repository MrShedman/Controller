#pragma once

#include "State.h"

#include "Button.h"
#include "Toggle.h"
#include "Slider.h"
#include "Container.h"
#include "Label.h"

class HomeState : public State
{
public:

	void setup() override;

	void handleTouch(const Touch& touch) override;

	void update() override;

private:

	Label label0;
	Label label1;
	Label label2;
	Label label3;
	Label label4;
	Label label5;

	Container<3> m_container;

	Button but1;

	Button but2;

	Button but3;

	uint32_t m_update_rate;
};