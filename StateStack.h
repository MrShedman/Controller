#pragma once

#include "State.h"

#include "HomeState.h"
#include "RadioConfigState.h"
#include "StickConfigState.h"
#include "IMUConfigState.h"
#include "VehicleConfigState.h"
#include "RingBuffer.h"

class StateStack
{
public:

	enum Action
	{
		Push,
		Pop
	};

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

		pushState(State::Home);

		applyPendingChanges();
	}

	void handleTouch(const Touch& touch)
	{
		m_current_state->handleTouch(touch);

		applyPendingChanges();
	}

	void update()
	{
		m_current_state->update();

		applyPendingChanges();
	}

	void popState()
	{
		m_pendingChange = PendingChange(Pop);
	}

	void pushState(State::ID new_state)
	{
		m_pendingChange = PendingChange(Push, new_state);
	}

	void clearScreen()
	{
		m_current_state->onSwitch();
	}

private:
	
	void applyPendingChanges()
	{
		if (m_pendingChange.needs_update)
		{
			State* old_state = m_current_state;

			switch (m_pendingChange.action)
			{
			case Push:
				m_history.push(m_pendingChange.stateID);
				m_current_state = m_states[m_pendingChange.stateID];
				break;

			case Pop:
				m_history.pop();
				m_current_state = m_states[*m_history.head()];
				break;
			}

			if (old_state != m_current_state)
			{
				clearScreen();
			}

			m_pendingChange.needs_update = false;
		}
	}


	struct PendingChange
	{
		PendingChange()
			:
			needs_update(false)
		{}

		PendingChange(Action action, State::ID stateID = State::Home)
			: 
			needs_update(true),
			action(action),
			stateID(stateID)
		{}

		bool needs_update;
		Action action;
		State::ID stateID;
	};

	PendingChange m_pendingChange;

	RingBuffer<State::ID, 10> m_history;

	State* m_current_state;

	State* m_states[State::NumStates];
};