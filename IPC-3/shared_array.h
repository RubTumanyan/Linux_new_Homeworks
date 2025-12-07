#ifndef SHARED_ARRAY_H
#define SHARED_ARRAY_H

#include <string>
#include <semaphore.h>

class shared_array {
private:
    int* data;
    size_t size;
    int shm_fd;
    sem_t* sem;

public:
    shared_array(const std::string& name, size_t size);
    ~shared_array();
    int& operator[](size_t index);
};

#endif

