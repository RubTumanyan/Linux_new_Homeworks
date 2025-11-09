#include "parallel_scheduler.hpp"
#include <iostream>
#include <chrono>

void task(int id){
    std::cout << "Task " << id << " start\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Task " << id << " end\n";
}

int main(){
    parallel_scheduler pool(3);
    for(int i=1;i<=10;i++){
        pool.run([i]{ task(i); });
    }
    std::this_thread::sleep_for(std::chrono::seconds(6));
}

