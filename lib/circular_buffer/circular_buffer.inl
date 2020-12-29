
template <class T, uint32_t max_size_>
void CircularBuffer<T, max_size_>::push(T item)
{
    buf_[head_] = item;

    if(full_)
    {
        tail_ = (tail_ + 1) % max_size_;
    }

    head_ = (head_ + 1) % max_size_;

    full_ = head_ == tail_;
}

template <class T, uint32_t max_size_>
T CircularBuffer<T, max_size_>::pop()
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

template <class T, uint32_t max_size_>
const T& CircularBuffer<T, max_size_>::front() const
{
    return buf_[tail_];
}

template <class T, uint32_t max_size_>
const T& CircularBuffer<T, max_size_>::back() const
{
    return buf_[head_];
}

template <class T, uint32_t max_size_>
T& CircularBuffer<T, max_size_>::operator[] (const uint32_t index)
{
    const uint32_t id = (index + tail_) % max_size_;
    return buf_[id];
}

template <class T, uint32_t max_size_>
const T& CircularBuffer<T, max_size_>::operator[] (const uint32_t index) const
{
    const uint32_t id = (index + tail_) % max_size_;
    return buf_[id];
}

template <class T, uint32_t max_size_>
void CircularBuffer<T, max_size_>::reset()
{
    head_ = tail_;
    full_ = false;
}

template <class T, uint32_t max_size_>
bool CircularBuffer<T, max_size_>::empty() const
{
    //if head and tail are equal, we are empty
    return (!full_ && (head_ == tail_));
}

template <class T, uint32_t max_size_>
bool CircularBuffer<T, max_size_>::full() const
{
    //If tail is ahead the head by 1, we are full
    return full_;
}

template <class T, uint32_t max_size_>
uint32_t CircularBuffer<T, max_size_>::capacity() const
{
    return max_size_;
}

template <class T, uint32_t max_size_>
uint32_t CircularBuffer<T, max_size_>::size() const
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