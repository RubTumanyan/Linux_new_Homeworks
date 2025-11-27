#include "parallel_scheduler.h"
#include <iostream>
#include <unistd.h>

void example_task(int id) {
    std::cout << "Task " << id << " started\n";
    sleep(1); // вместо std::this_thread::sleep_for
    std::cout << "Task " << id << " finished\n";
}

int main() {
    parallel_scheduler pool(3);

    for (int i = 0; i < 10; ++i) {
        pool.run(example_task, i);
    }

    sleep(5); 
    return 0;
}

