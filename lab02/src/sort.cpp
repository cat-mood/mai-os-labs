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

void* thread_sort(void* arg) {
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
        Piece less = {p->mas, p->start, j};
        thread_sort(&less);
    }
    if (i < p->end) {
        Piece more = {p->mas, i, p->end};
        thread_sort(&more);
    }

    return 0;
}

int* merge(int* a, size_t size_a, int* b, size_t size_b) {
    size_t size_res = size_a + size_b;
    int* res = new int[size_res];
    int i = 0, j = 0, k = 0;
    while (i < size_a || j < size_b) {
        if (i >= size_a) {
            res[k] = b[j];
            ++j;
        } else if (j >= size_b) {
            res[k] = a[i];
            ++i;
        } else {
            if (a[i] < b[j]) {
                res[k] = a[i];
                ++i;
            } else {
                res[k] = b[j];
                ++j;
            }
        }
        ++k;
    }
    return res;
}

void sort(int* array, int n, int threads) {
    Piece p[threads];
    pthread_t tid[threads];
    for (int i = 0; i < threads; ++i) {
        int* array_piece = new int[n / threads];
        int counter = 0;
        for (int j = i * (n / threads); j < (i + 1) * (n / threads); ++j) {
            array_piece[counter] = array[j];
            ++counter;
        }
        p[i] = Piece{array_piece, 0, n / threads - 1};
        create_thread(&tid[i], NULL, thread_sort, &p[i]);
    }
    for (int i = 0; i < threads; ++i) {
        pthread_join(tid[i], NULL);
    }
    for (int i = 0; i < threads; ++i) {
        int counter = 0;
        for (int j = i * (n / threads); j < (i + 1) * (n / threads); ++j) {
            array[j] = p[i].mas[counter];
            ++counter;
        }
    }
    // Piece res = Piece{array, 0, n - 1};
    // thread_sort(&res);
    int* res = new int[0];
    size_t res_size = 0;
    for (int i = 0; i < threads; ++i) {
        res = merge(res, res_size, p[i].mas, n / threads);
        res_size += n / threads;
    }
    for (int i = 0; i < n; ++i) {
        array[i] = res[i];
    }
}
