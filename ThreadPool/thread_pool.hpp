// thread_pool.hpp

#pragma once
#ifndef __THREAD_POOL_HPP__
#define __THREAD_POOL_HPP__

#include <pthread.h>
#include <queue>
#include <cassert>

#include "condition.hpp"
#include "mutex.hpp"

namespace akashi {

class ThreadPool {

    // task types
    using func_t = void (*)(void *);
    using task_t = struct { func_t func; void* arg; };

    // task queue
    std::queue<task_t> task_queue_;

    // pthread_t array
    pthread_t* thread_pool_;

    // thread num
    const size_t thread_num_;

    // mutex
    Mutex mutex_;

    // condition variable
    Condition cond_;

    // running flag
    bool running_;

private:

    // thread routine
    static void* thread_routine(void* thread_pool) {
        // get thread pool "this" pointer
        ThreadPool* tp = static_cast<ThreadPool*>(thread_pool);
        // if is running
        while (tp->running_) {
            
            tp->mutex_.lock(); // lock queue
            
            // wait for task
            while (tp->running_ && tp->task_queue_.empty())
                tp->cond_.wait(tp->mutex_);

            if (tp->task_queue_.empty()) {
                tp->mutex_.unlock(); // unlock queue
                continue;
            }
            // get task
            task_t task = tp->task_queue_.front();
            tp->task_queue_.pop();

            tp->mutex_.unlock(); // unlock queue

            // execute task
            task.func(task.arg);

        } // while
        return nullptr;
    }

public:

    // constructor
    ThreadPool(size_t thread_num = 4) :thread_num_(thread_num) {
        // new pthread_t array
        thread_pool_ = new pthread_t[thread_num];
        assert(thread_pool_ != nullptr);
    }

    // destructor
    ~ThreadPool() {
        // shutdown
        if (running_) this->shutdown();
    }

    void run() {
        // set running flag
        running_ = true;

        // create threads
        for (size_t i = 0; i < thread_num_; ++i)
            pthread_create(thread_pool_ + i, nullptr, thread_routine, (void*)this);
    }

    // shutdown
    void shutdown() {
        // is shutdown?
        if (!running_) return;
        
        // set running flag
        running_ = false;

        // broadcast to all threads
        cond_.broadcast();

        // join threads
        for (size_t i = 0; i < thread_num_; ++i)
            pthread_join(thread_pool_[i], nullptr);

        // free pthread_t array
        delete[] thread_pool_;
        thread_pool_ = nullptr;
    }

    // is_running
    inline bool is_running() {
        mutex_.lock();

        // get running flag
        bool ret = running_;
        
        mutex_.unlock();
        return ret;
    }

    // push task
    inline void push(func_t task, void* arg) {
        mutex_.lock();

        // push task to task queue
        task_queue_.push({task, arg});
        
        // signal to thread
        cond_.signal();
        
        mutex_.unlock();
    }

}; // class ThreadPool

} // namespace akashi

#endif // __THREAD_POOL_HPP__