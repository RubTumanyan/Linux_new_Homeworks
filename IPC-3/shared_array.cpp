#include "shared_array.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>

shared_array::shared_array(const std::string& name, size_t size) : size(size) {
    std::string shm_name = "/" + name;
    std::string sem_name = "/" + name + "_sem";

    shm_fd = shm_open(shm_name.c_str(), O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) { perror("shm_open"); exit(1); }

    if (ftruncate(shm_fd, size * sizeof(int)) == -1) { perror("ftruncate"); exit(1); }

    data = (int*) mmap(nullptr, size * sizeof(int),
                       PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED) { perror("mmap"); exit(1); }

    sem = sem_open(sem_name.c_str(), O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) { perror("sem_open"); exit(1); }
}

shared_array::~shared_array() {
    munmap(data, size * sizeof(int));
    close(shm_fd);
}

int& shared_array::operator[](size_t index) {
    if (index >= size) { std::cerr << "Index out of range\n"; exit(1); }
    return data[index];
}


