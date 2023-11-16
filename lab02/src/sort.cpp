#include "sort.h"
#include <pthread.h>
#include <iostream>
#include <algorithm>
#include <memory>

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
        Piece less = {p->mas, j - p->start + 1, p->start, j};
        thread_sort(&less);
    }
    if (i < p->end) {
        Piece more = {p->mas, p->end - i + 1, i, p->end};
        thread_sort(&more);
    }

    return 0;
}

std::shared_ptr<int[]> merge(std::shared_ptr<int[]> a, int size_a, std::shared_ptr<int[]> b, int size_b) {
    size_t size_res = size_a + size_b;
    std::shared_ptr<int[]> res = std::make_shared<int[]>(size_res);
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

void sort(std::shared_ptr<int[]> array, int n, int threads) {
    Piece p[threads];
    pthread_t tid[threads];
    int num_of_elems = n;
    int counter = 0;
    for (int i = 0; i < threads; ++i) {
        int size_array_piece = (i == threads - 1) ? num_of_elems : (n / threads);
        num_of_elems -= n / threads;
        std::shared_ptr<int[]> array_piece = std::make_shared<int[]>(size_array_piece);
        for (int j = 0; j < size_array_piece; ++j) {
            array_piece[j] = array[counter];
            ++counter;
        }
        p[i] = Piece{array_piece, size_array_piece, 0, size_array_piece - 1};
        create_thread(&tid[i], NULL, thread_sort, &p[i]);
    }
    for (int i = 0; i < threads; ++i) {
        pthread_join(tid[i], NULL);
    }
    std::shared_ptr<int[]> res = p[0].mas;
    int res_size = p->size;
    for (int i = 1; i < threads; ++i) {
        res = merge(res, res_size, p[i].mas, p[i].size);
        res_size += p[i].size;
    }
    for (int i = 0; i < n; ++i) {
        array[i] = res[i];
    }
}
