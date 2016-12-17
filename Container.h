
#pragma once

#include "Arduino.h"
#include "Component.h"
#include "GUIConstants.h"


template <uint8_t NUM>
class Container : public Component
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

		bubble_sort(m_children, index);
	}
	
	bool handleTouch(const Touch& t) override
	{
		for (uint8_t i = 0; i < size(); ++i)
		{
			if (m_children[i]->handleTouch(t))
			{
				return true;
			}
		}

		return false;
	}

	void draw(bool force_draw = false) override
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
	uint8_t index;
	const uint8_t m_size;
	Component* m_children[NUM];

	void bubble_sort(Component* list[], uint8_t size)
	{
		Component* temp;
		for (int i = 0; i<size; i++)
		{
			for (int j = size - 1; j>i; j--)
			{
				if (list[j]->getPriority() < list[j - 1]->getPriority())
				{
					temp = list[j - 1];
					list[j - 1] = list[j];
					list[j] = temp;
				}
			}
		}
	}
};