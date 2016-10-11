#pragma once

#include <Arduino.h>

template <typename T, uint16_t cap>
class RingBuffer
{
public:

	RingBuffer()
		:
		m_head(m_buffer),
		m_tail(m_buffer),
		m_capacity(cap),
		m_size(0)
	{}

	void push(T value)
	{
		*m_head++ = value;
		m_size++;

		if (m_size == m_capacity + 1)
		{
			m_size = m_capacity;

			m_tail = m_head + 1;
		}
		if (m_head == (m_buffer + m_capacity))
		{
			m_head = m_buffer;
			m_tail = m_head + 1;
		}
		if (m_tail == (m_buffer + m_capacity))
		{
			m_tail = m_buffer;
		}
	}

	T* head()
	{
		return m_head;
	}

	T* tail()
	{
		return m_tail;
	}

	const T* next(const T* ptr)
	{
		++ptr;

		if (ptr == (m_buffer + m_size))
		{
			ptr = m_buffer;
		}

		return ptr;
	}

	bool full() const
	{
		return (m_size == m_capacity);
	}

	uint16_t capacity() const
	{
		return m_capacity;
	}

	uint16_t size() const
	{
		return m_size;
	}

	const T operator[](uint16_t index) const
	{
		return m_buffer[index];
	}

	T operator[](uint16_t index)
	{
		return m_buffer[index];
	}

private:

	T m_buffer[cap];
	T *m_head;
	T *m_tail;
	uint16_t m_size;
	uint16_t m_capacity;
};