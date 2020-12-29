#pragma once

#include <Arduino.h>

template <class T, uint32_t max_size_>
class CircularBuffer
{
public:

	void push(T item);

	T pop();

	const T& front() const;

	const T& back() const;

	T& operator[] (const uint32_t index);

	const T& operator[] (const uint32_t index) const;

	void reset();

	bool empty() const;

	bool full() const;

	uint32_t capacity() const;

	uint32_t size() const;

private:

    T buf_[max_size_];
	uint32_t head_ = 0;
	uint32_t tail_ = 0;
	bool full_ = 0;
};

#include "circular_buffer.inl"