#pragma once

#include <memory>

struct Piece{
    std::shared_ptr<int[]> mas;
    int size;
    int start;
    int end;
};

void sort(std::shared_ptr<int[]> array, int n, int threads);
