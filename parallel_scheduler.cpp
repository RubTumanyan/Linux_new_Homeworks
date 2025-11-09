#include "parallel_scheduler.hpp"

parallel_scheduler::parallel_scheduler(size_t capacity) {
    for(size_t i=0;i<capacity;i++){
        threads.emplace_back([this](){
            while(true){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this]{ return stop || !tasks.empty(); });
                    if(stop && tasks.empty()) return;
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }
}

void parallel_scheduler::run(std::function<void()> task){
    {
        std::lock_guard<std::mutex> lock(mtx);
        tasks.push(std::move(task));
    }
    cv.notify_one();
}

parallel_scheduler::~parallel_scheduler(){
    {
        std::lock_guard<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();
    for(auto &t: threads) t.join();
}

