#pragma once

struct Piece{
    int* mas;
    int start;
    int end;
};

void sort(int* array, int n, int threads);
