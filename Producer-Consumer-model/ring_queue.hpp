// ring_queue.hpp

#pragma once
#ifndef __RING_QUEUE_HPP__
#define __RING_QUEUE_HPP__

namespace akashi {

template <class T>
class RingQueue {

    T* data_;
    size_t front_;
    size_t rear_;
    size_t capacity_;

public:

    // constructor
    RingQueue(size_t size = 5) :data_(new T[size]), front_(0), rear_(0), capacity_(size) {}

    // destructor
    ~RingQueue() {
        delete[] data_;
        data_ = nullptr;
    }

    inline void pop() {
        front_ = (front_ + 1) % capacity_;
    }

    inline void push(const T& val) {
        data_[rear_] = val;
        rear_ = (rear_ + 1) % capacity_;
    }

    inline size_t size() const {
        return (rear_ - front_ + capacity_) % capacity_;
    }

    inline T front() const {
        return data_[front_];
    }
    
}; // class RingQueue

} // namespace akashi

#endif