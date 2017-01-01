#pragma once

#include "State.h"
#include "Graph.h"
#include "Series.h"

class StickConfigState : public State
{
public:

	void setup() override;

	void handleTouch(const Touch& touch) override;

	void update() override;

private:
	
	Graph<4> m_graph;
	Series m_series1;
	Series m_series2;
	Series m_series3;
	Series m_series4;
};