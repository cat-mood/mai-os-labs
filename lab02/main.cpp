#include "sort.h"
#include <iostream>
#include <chrono>

using namespace std::chrono;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        perror("Using: ./lab02_exe num_of_threads");
        exit(-1);
    }
    int n;
    std::cout << "Enter the number of elements: ";
    std::cin >> n;
    int mas[n];
    std::cout << "Fill array: ";
    for (int i = 0; i < n; ++i) {
        std::cin >> mas[i];
    }
    Piece p = {mas, 0, n - 1, atoi(argv[1])};
    system_clock::time_point start = system_clock::now();
    parallel_quick_sort(p);
    system_clock::time_point end = system_clock::now();
    duration<double> sec = end - start;
    std::cout << "Result: ";
    for (int i = 0; i < n; ++i) {
        std::cout << mas[i] << ' ';
    }
    std::cout << std::endl;
    std::cout << sec.count() << " s" << std::endl;

    return 0;
}
