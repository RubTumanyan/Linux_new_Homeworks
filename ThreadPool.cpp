#include "ThreadPool.hpp"

namespace SimpleNet {

ThreadPool::ThreadPool(size_t threads) {
    for (size_t i = 0; i < threads; ++i) {
        workers_.emplace_back([this]() {
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock lock(mutex_);
                    cv_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });

                    if (stop_ && tasks_.empty())
                        return; 

                    task = std::move(tasks_.front());
                    tasks_.pop();
                }

                task();
            }
        });
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::lock_guard lock(mutex_);
        tasks_.push(std::move(task));
    }
    cv_.notify_one();
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard lock(mutex_);
        stop_ = true;
    }
    cv_.notify_all();

    for (auto& t : workers_)
        t.join();
}

}
