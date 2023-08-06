// mutex.hpp

#pragma once
#ifndef __MUTEX_HPP__
#define __MUTEX_HPP__

#include <pthread.h>

namespace akashi {

class Mutex {
    
    // mutex
    pthread_mutex_t mutex;

public:

    // init mutex
    Mutex() {
        pthread_mutex_init(&mutex, nullptr);
    }

    // destroy mutex
    ~Mutex() {
        pthread_mutex_destroy(&mutex);
    }

    // lock mutex
    inline void lock() {
        pthread_mutex_lock(&mutex);
    }

    // unlock mutex
    inline void unlock() {
        pthread_mutex_unlock(&mutex);
    }

    // try lock mutex
    inline void trylock() {
        pthread_mutex_trylock(&mutex);
    }

    // get mutex
    inline pthread_mutex_t* get() {
        return &mutex;
    }

}; // class Mutex

} // namespace akashi

#endif // __MUTEX_HPP__
