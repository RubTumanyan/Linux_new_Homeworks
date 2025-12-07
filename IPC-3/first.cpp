#include "shared_array.h"
#include <iostream>
#include <unistd.h>
#include <semaphore.h>

int main() {
    shared_array arr("myarray", 5);
    sem_t* sem = sem_open("/myarray_sem", 0);

    while (true) {
        sem_wait(sem);
        for (int i = 0; i < 5; i++) arr[i]++;
        std::cout << "FIRST: ";
        for (int i = 0; i < 5; i++) std::cout << arr[i] << " ";
        std::cout << "\n";
        sem_post(sem);
        sleep(1);
    }
}

