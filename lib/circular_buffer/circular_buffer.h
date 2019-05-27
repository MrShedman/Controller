#pragma once

#include <cstdint>
#include <array>

template <class T, uint32_t max_size_>
class CircularBuffer
{
public:

	void push(T item)
	{
		buf_[head_] = item;

		if(full_)
		{
			tail_ = (tail_ + 1) % max_size_;
		}

		head_ = (head_ + 1) % max_size_;

		full_ = head_ == tail_;
	}

	T pop()
	{
		if(empty())
		{
			return T();
		}

		//Read data and advance the tail (we now have a free space)
		auto val = buf_[tail_];
		full_ = false;
		tail_ = (tail_ + 1) % max_size_;

		return val;
	}

	const T& front() const
	{
		return buf_[tail_];
	}

	const T& back() const
	{
		return buf_[head_];
	}

	T& operator[] (const uint32_t index)
	{
		const uint32_t id = (index + tail_) % max_size_;
		return buf_[id];
	}

	const T& operator[] (const uint32_t index) const
	{
		const uint32_t id = (index + tail_) % max_size_;
		return buf_[id];
	}

	void reset()
	{
		head_ = tail_;
		full_ = false;
	}

	bool empty() const
	{
		//if head and tail are equal, we are empty
		return (!full_ && (head_ == tail_));
	}

	bool full() const
	{
		//If tail is ahead the head by 1, we are full
		return full_;
	}

	uint32_t capacity() const
	{
		return max_size_;
	}

	uint32_t size() const
	{
		uint32_t size = max_size_;

		if(!full_)
		{
			if(head_ >= tail_)
			{
				size = head_ - tail_;
			}
			else
			{
				size = max_size_ + head_ - tail_;
			}
		}

		return size;
	}

private:

    std::array<T, max_size_> buf_;
	uint32_t head_ = 0;
	uint32_t tail_ = 0;
	bool full_ = 0;
};