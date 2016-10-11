
#pragma once

#include "State.h"

#include "Button.h"
#include "Toggle.h"
#include "Slider.h"
#include "Container.h"
#include "Graph.h"
#include "Series.h"

class IMUConfigState : public State
{
public:

	void setup() override;

	void handleTouch(const Touch& touch) override;

	void update() override;

private:

	uint32_t m_graph_last_draw_time;

	Graph<4> m_graph;
	Series m_series1;
	Series m_series2;
	Series m_series3;
	Series m_series4;

	Container<2> m_container;

	Button button_back;
	Slider slider;
};