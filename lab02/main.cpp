#include "sort.h"
#include "threadscount.h"
#include <iostream>
#include <chrono>
#include <memory>

using namespace std::chrono;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        perror("Using: ./lab02_exe num_of_threads");
        exit(-1);
    }
    int n;
    std::cout << "Enter the number of elements: ";
    std::cin >> n;
    std::shared_ptr<int[]> mas = std::make_shared<int[]>(n);
    std::cout << "Fill array: ";
    for (int i = 0; i < n; ++i) {
        std::cin >> mas[i];
    }
    int threads(atoi(argv[1]));
    auto start = std::chrono::high_resolution_clock::now();
    sort(mas, n, threads);
    auto end = std::chrono::high_resolution_clock::now();
    duration<double> sec = end - start;
    std::cout << "Result: ";
    std::cout << sec.count() << " s" << std::endl;
    // for (int i = 0; i < n; ++i) {
    //     std::cout << mas[i] << ' ';
    // }
    // std::cout << std::endl;

    return 0;
}
