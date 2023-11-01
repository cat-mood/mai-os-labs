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

void* sort(void* arg) {
    Piece* p = (Piece*) arg;
    int i = p->start;
    int j = p->end;
    int mid = p->mas[(i + j) / 2];
    int swaps = 0;
    do {
        while (p->mas[i] < mid) {
            ++i;
        }
        while (p->mas[j] > mid) {
            --j;
        }
        if (i <= j) {
            std::swap(p->mas[i], p->mas[j]);
            ++swaps;
            ++i;
            --j;
        }
    } while (i <= j);
    if (p->start < j) {
        Piece less = {p->mas, p->start, j, p->threads};
        if (p->threads->get_count() == 1 || p->threads->get_count() % 2 == 0) {
            sort(&less);
        } else {
            --*p->threads;
            pthread_t thread;
            create_thread(&thread, NULL, sort, &less);
            pthread_join(thread, NULL);
        }
    }
    if (i < p->end) {
        Piece more = {p->mas, i, p->end, p->threads};
        if (p->threads->get_count() == 1 || p->threads->get_count() % 2 == 1) {
            sort(&more);
        } else {
            --*p->threads;
            pthread_t thread;
            create_thread(&thread, NULL, sort, &more);
            pthread_join(thread, NULL);
        }
    }

    return 0;
}
