#include <iostream>
#include <string>
#include <chrono>
using namespace std;

#include "thread_pool.hpp"
using namespace akashi;

#include <unistd.h>

std::chrono::_V2::system_clock::time_point start, end;

const size_t TASK_NUM = 20000;
const size_t TASK_CNT = 500000;

// void* func(void* arg) {
//     for (int i = 0; i < TASK_CNT; ++i);
//     return nullptr;
// }

// int main() {
//     size_t count = 0;
//     pthread_t tid[TASK_NUM];
//     start = std::chrono::high_resolution_clock::now();
//     for (size_t i = 0; i < TASK_NUM; ++i) {
//         pthread_create(&tid[i], NULL, func, (void*)i);
//     }
//     for (size_t i = 0; i < TASK_NUM; ++i) {
//         pthread_join(tid[i], nullptr);
//     }
//     ::end = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(::end - start);
//     std::cout << "普通调度: " << duration.count() / 1e6 << "s" << std::endl;
//     return 0;
// }

void func(void* arg) {
    for (int i = 0; i < TASK_CNT; ++i);
    if ((size_t)arg == TASK_NUM) {
        ::end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(::end - start);
        std::cout << "线程池: " << duration.count() / 1e6 << "s" << std::endl;
        exit(0);
    }
}

int main() {
    ThreadPool pool(4);
    pool.run();
    size_t count = 0;
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < TASK_NUM; ++i) {
        pool.push(func, (void*)++count);
    }
    getchar();
    pool.shutdown();
    return 0;
}

