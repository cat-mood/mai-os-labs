#include <iostream>
#include "mutex.h"

using namespace bc;

int main() {
    std::cout << "Starting client" << std::endl;
    Mutex mtx("mutex", MutexFlag::connect);
    MemoryMap<int> data("data", 3, ModeFlags::write);
    mtx.unlock();   // say server i'm connected
    mtx.lock();
    for (int i = 0; i < 3; ++i) {
        std::cin >> data[i];
    }
    mtx.unlock();

    return 0;
}
