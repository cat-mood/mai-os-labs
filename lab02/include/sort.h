#pragma once

struct Piece{
    int* mas;
    int start;
    int end;
    int threads;
};

void parallel_quick_sort(Piece p);
