#pragma once

#include "State.h"
#include "Label.h"
#include "Rect.h"
#include "ScrollView.h"

class StickConfigState : public State
{
public:

	void setup() override;

	void handleTouch(const Touch& touch) override;

	void update() override;

private:
	
	ScrollView scrollview;

	Rect rect;

	Label label0;
	Label label1;
	Label label2;
	Label label3;
	Label label4;

	uint32_t m_update_rate;
};