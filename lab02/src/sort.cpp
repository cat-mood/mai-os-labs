#include "sort.h"
#include <pthread.h>
#include <iostream>
#include <algorithm>

void create_thread(pthread_t* thread, const pthread_attr_t* attr, void *(*start)(void *), void* arg) {
    if (pthread_create(thread, attr, start, arg) != 0) {
        perror("create_thread error!");
        exit(-1);
    }
}

void *thread_sort(void* arg) {
    Piece* p = (Piece*) arg;
    parallel_quick_sort(*p);

    return 0;
}

void parallel_quick_sort(Piece p) {
    int i = p.start;
    int j = p.end;
    int mid = p.mas[(i + j) / 2];
    do {
        while (p.mas[i] < mid) {
            ++i;
        }
        while (p.mas[j] > mid) {
            --j;
        }
        if (i <= j) {
            std::swap(p.mas[i], p.mas[j]);
            ++i;
            --j;
        }
    } while (i <= j);
    int threads = p.threads;
    if (p.start < j) {
        if (threads == 1) {
            parallel_quick_sort({p.mas, p.start, j, 1});
        } else {
            --threads;
            pthread_t thread;
            Piece less = {p.mas, p.start, j, threads};
            create_thread(&thread, NULL, thread_sort, &less);
            pthread_join(thread, NULL);
        }
    }
    if (i < p.end) {
        Piece more = {p.mas, i, p.end, threads};
        parallel_quick_sort(more);
    }
}
