
#pragma once

#include "Arduino.h"
#include "Component.h"

template <uint8_t NUM>
class Container
{
public:

	Container()
		:
		m_size(NUM),
		index(0)
	{

	}

	void pack(Component* component)
	{
		if (index >= m_size)
		{
			Serial.println("Container size too small!");
			return;
		}

		m_children[index++] = component;
	}
	
	uint8_t size() const
	{
		return index;
	}

	const Component* operator[](int index) const
	{
		return m_children[index];
	}

	Component* operator[](int index)
	{
		return m_children[index];
	}

private:
	uint8_t index;
	const uint8_t m_size;
	Component* m_children[NUM];
};