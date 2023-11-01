#pragma once
#include <pthread.h>

class ThreadsCount {
public:
    ThreadsCount();
    ThreadsCount(int count);
    ~ThreadsCount();
    int get_count();
    void set_count(int count);
    ThreadsCount& operator--();
private:
    int _count;
    pthread_mutex_t _mutex;
};
