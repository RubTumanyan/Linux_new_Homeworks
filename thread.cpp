#include <iostream>
#include <thread> // библиотека для потоков

void print_numbers(int id) {
    for (int i = 1; i <= 5; ++i) {
        std::cout << "Thread " << id << ": " << i << std::endl;
    }
}

int main() {
    // Создаём два потока
    std::thread t1(print_numbers, 1);
    std::thread t2(print_numbers, 2);

    // Ждём завершения потоков
    t1.join();
    t2.join();

    std::cout << "Main thread finished." << std::endl;
    return 0;
}

