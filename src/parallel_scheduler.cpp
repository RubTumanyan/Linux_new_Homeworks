#include "parallel_scheduler.h"

parallel_scheduler::parallel_scheduler(size_t max_threads)
    : max_threads_(max_threads), stop_(false)
{
    threads_.resize(max_threads_);
    for (size_t i = 0; i < max_threads_; ++i) {
        pthread_create(&threads_[i], nullptr, thread_worker, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    stop_ = true;
    cv_.notify_all();

    for (auto& t : threads_) {
        pthread_join(t, nullptr);
    }
}

void* parallel_scheduler::thread_worker(void* arg) {
    parallel_scheduler* scheduler = static_cast<parallel_scheduler*>(arg);
    scheduler->worker();
    return nullptr;
}

void parallel_scheduler::worker() {
    while (!stop_) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            cv_.wait(lock, [this]{ return stop_ || !tasks_.empty(); });
            if (stop_ && tasks_.empty())
                return;
            task = std::move(tasks_.front());
            tasks_.pop();
        }
        task();
    }
}

