#pragma once

#include "Touch.h"

class State
{
public:

	friend class StateStack;

	enum ID : uint8_t
	{
		Home,
		RadioConfig,
		StickConfig,
		IMUConfig,
		VehicleConfig,
		NumStates
	};

	virtual ~State(){}

	virtual void setup() = 0;
	virtual void handleTouch(const Touch& touch) = 0;
	virtual void update() = 0;

protected:

	bool m_force_redraw = true;
	void onSwitch();
};