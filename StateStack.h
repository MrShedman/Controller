#pragma once

#include "State.h"

#include "HomeState.h"
#include "RadioConfigState.h"
#include "StickConfigState.h"
#include "IMUConfigState.h"
#include "VehicleConfigState.h"

class StateStack
{
public:

	~StateStack()
	{
		for (uint8_t i = 0; i < State::NumStates; ++i)
		{
			delete m_states[i];
		}

		m_current_state = nullptr;
	}

	void begin()
	{
		m_states[State::Home] = new HomeState();
		m_states[State::RadioConfig] = new RadioConfigState();
		m_states[State::StickConfig] = new StickConfigState();
		m_states[State::IMUConfig] = new IMUConfigState();
		m_states[State::VehicleConfig] = new VehicleConfigState();

		for (uint8_t i = 0; i < State::NumStates; ++i)
		{
			m_states[i]->setup();
		}

		m_current_state = m_states[State::Home];
		m_pending_change = false;
	}

	bool handleTouch(const Touch& touch)
	{
		m_current_state->handleTouch(touch);

		applyPendingChanges();
	}

	void update()
	{
		m_current_state->update();

		applyPendingChanges();
	}

	void applyPendingChanges()
	{
		if (m_pending_change)
		{
			m_current_state = m_states[m_new_state];
			m_current_state->onSwitch();
			m_pending_change = false;
		}
	}

	void requestStateChange(State::ID new_state)
	{
		m_pending_change = true;
		m_new_state = new_state;
	}

private:
	
	bool m_pending_change;
	State::ID m_new_state;

	State* m_current_state;

	State* m_states[State::NumStates];
};