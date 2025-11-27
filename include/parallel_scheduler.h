#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <pthread.h>
#include <queue>
#include <functional>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>

class parallel_scheduler {
public:
    explicit parallel_scheduler(size_t max_threads);
    ~parallel_scheduler();

    template<typename Func, typename Arg>
    void run(Func f, Arg arg);

private:
    static void* thread_worker(void* arg);
    void worker();

    size_t max_threads_;
    std::vector<pthread_t> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable cv_;
    std::atomic<bool> stop_;
};

#include "parallel_scheduler.tpp"

#endif

