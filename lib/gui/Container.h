
#pragma once

#include "Arduino.h"
#include "Component.h"

template <uint8_t NUM>
class Container
{
public:

	Container()
		:
		index(0)
	{}

	void pack(Component* component)
	{
		if (index >= NUM)
		{
			Serial.println("Container size too small!");
		}
		else
		{
			m_children[index++] = component;
		}
	}
	
	bool handleTouch(const Touch& t)
	{
		uint8_t max_level = getMaxLevel();

		for (uint8_t i = 0; i < size(); ++i)
		{
			if (levels[i] == max_level)
			{
				if (m_children[i]->handleTouch(t))
				{
					return true;
				}
			}
		}

		return false;
	}

	void draw(bool force_draw = false)
	{
		for (uint8_t i = 0; i < size(); ++i)
		{
			m_children[i]->draw(force_draw);
		}
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

	uint8_t getMaxLevel()
	{
		uint8_t max_level = 0;

		for (uint8_t i = 0; i < size(); ++i)
		{
			uint8_t l = m_children[i]->getChildLevel();

			if (l > max_level)
			{
				max_level = l;
			}
			levels[i] = l;
		}

		return max_level;
	}

	uint8_t index;
	Component* m_children[NUM];
	uint8_t levels[NUM];
};