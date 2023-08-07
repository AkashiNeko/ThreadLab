#include <unistd.h>
#include <cstdio>
#include <semaphore.h>
#include <iostream>
using namespace std;

#include "ring_queue.hpp"
#include "semaphore.hpp"
#include "mutex.hpp"
using namespace akashi;

const size_t MAX_SPACE = 3;
const size_t PRODUCERS_NUM = 3;
const size_t CONSUMERS_NUM = 2;
const size_t PRODUCERS_MAKE_TASK_TIME = 3; // seconds
const size_t CONSUMERS_DO_TASK_TIME = 2; // seconds

Mutex mutex;
Semaphore space_sem(MAX_SPACE);
Semaphore data_sem(0);

Mutex printer; // screen printers are also a scarce resource

pthread_t producers_tid[PRODUCERS_NUM], consumers_tid[CONSUMERS_NUM];

inline string sem_values() {
    return "space=" + to_string(space_sem.value()) + ",data=" + to_string(data_sem.value());
}

// consumers do task
void do_task(int arg) {

    printer.lock();
    cout << "task " << arg << " start.. " << endl;
    printer.unlock();
    
    // do something..
    sleep(CONSUMERS_DO_TASK_TIME);
    
    printer.lock();
    cout << "task " << arg << " done.. " << endl;
    printer.unlock();
}

// producers make task
inline int get_task() {
    static int task_id = 0;
    return task_id++;
}

void* producer(void* arg) {
    auto queue = (RingQueue<int>*)arg;
    cout << "producer start.. tid = " << pthread_self() << endl;
    while (true) {
        // produce task
        int task = get_task();

        // space P
        space_sem.wait();

        // lock
        mutex.lock();

        // push task
        queue->push(task);

        printer.lock();
        cout << "task " << task << " pushed.. " << endl;
        printer.unlock();

        // unlock
        mutex.unlock();

        // data V
        data_sem.post();

        sleep(PRODUCERS_MAKE_TASK_TIME);
    }
    return nullptr;
}

void* consumer(void* arg) {
    auto queue = (RingQueue<int>*)arg;
    cout << "consumer start.. tid = " << pthread_self() << endl;
    while (true) {
        // data P
        data_sem.wait();

        // lock
        mutex.lock();

        // get task
        int task = queue->front();
        queue->pop();
        
        // unlock
        mutex.unlock();

        // space V
        space_sem.post();

        // do task
        do_task(task);
    }
    return nullptr;
}

int main() {

    auto queue = new RingQueue<int>(MAX_SPACE);

    // create producers and consumers
    for (size_t i = 0; i < PRODUCERS_NUM; ++i)
        pthread_create(producers_tid + i, nullptr, producer, (void*)queue);
    for (size_t i = 0; i < CONSUMERS_NUM; ++i)
        pthread_create(consumers_tid + i, nullptr, consumer, (void*)queue);


    while (true) {
        cout << "\033[32m" << sem_values() << "\033[0m\r";
        fflush(stdout);
        usleep(500000);
    }


    // join producers and consumers
    for (size_t i = 0; i < PRODUCERS_NUM; ++i)
        pthread_join(producers_tid[i], nullptr);
    for (size_t i = 0; i < CONSUMERS_NUM; ++i)
        pthread_join(consumers_tid[i], nullptr);

    return 0;
}