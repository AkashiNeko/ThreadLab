// semaphore.hpp

#pragma once
#ifndef __SEMAPHORE_HPP__
#define __SEMAPHORE_HPP__

#include <pthread.h>

namespace akashi {

class Semaphore {

    // semaphore
    sem_t sem;

public:

    // constructor
    Semaphore(int count = 0) {
        sem_init(&sem, 0, count);
    }

    // destructor
    ~Semaphore() {
        sem_destroy(&sem);
    }

    // P
    inline void wait() {
        sem_wait(&sem);
    }
    inline void p() {
        this->wait();
    }

    // V
    inline void post() {
        sem_post(&sem);
    }
    inline void v() {
        this->post();
    }

    // get value
    inline int value() {
        int value = 0;
        sem_getvalue(&sem, &value);
        return value;
    }

}; // class Semaphore

} // namespace akashi

#endif // __SEMAPHORE_HPP__