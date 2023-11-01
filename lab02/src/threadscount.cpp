#include "threadscount.h"

ThreadsCount::ThreadsCount() : _count{0} {
    pthread_mutex_init(&_mutex, NULL);
}

ThreadsCount::ThreadsCount(int count) : _count{count} {
    pthread_mutex_init(&_mutex, NULL);
}

ThreadsCount::~ThreadsCount() {
    _count = 0;
    pthread_mutex_destroy(&_mutex);
}

int ThreadsCount::get_count() {
    int res;
    pthread_mutex_lock(&_mutex);
    res = _count;
    pthread_mutex_unlock(&_mutex);
    return res;
}

void ThreadsCount::set_count(int count) {
    pthread_mutex_lock(&_mutex);
    _count = count;
    pthread_mutex_unlock(&_mutex);
}

ThreadsCount& ThreadsCount::operator--() {
    pthread_mutex_lock(&_mutex);
    --_count;
    pthread_mutex_unlock(&_mutex);
    return *this;
}
