template<typename Func, typename Arg>
void parallel_scheduler::run(Func f, Arg arg) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        tasks_.emplace([f, arg](){ f(arg); });
    }
    cv_.notify_one();
}

