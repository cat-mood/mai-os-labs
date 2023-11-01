#pragma once
#include "threadscount.h"
#include <string>

struct Piece{
    int* mas;
    int start;
    int end;
    ThreadsCount* threads;
    std::string buf;
};

void parallel_quick_sort(Piece p);
void* sort(void* arg);
