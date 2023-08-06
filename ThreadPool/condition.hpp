// condition.hpp

#pragma once
#ifndef __CONDITION_HPP__
#define __CONDITION_HPP__

#include <pthread.h>
#include "mutex.hpp"

namespace akashi {

class Condition {

    // condition variable
    pthread_cond_t cond;

public:

    // init condition variable
    Condition() {
        pthread_cond_init(&cond, nullptr);
    }

    // destroy condition variable
    ~Condition() {
        pthread_cond_destroy(&cond);
    }

    // wait for condition
    inline void wait(Mutex& mutex) {
        pthread_cond_wait(&cond, mutex.get());
    }

    inline void wait(pthread_mutex_t* pmutex) {
        pthread_cond_wait(&cond, pmutex);
    }

    // signal condition
    inline void signal() {
        pthread_cond_signal(&cond);
    }

    // broadcast condition
    inline void broadcast() {
        pthread_cond_broadcast(&cond);
    }

    // get condition variable
    inline pthread_cond_t* get() {
        return &cond;
    }

}; // class Condition

} // namespace akashi

#endif // __CONDITION_HPP__
