#include <iostream>
#include "mutex.h"

using namespace bc;

int main() {
    std::cout << "Starting server" << std::endl;
    Mutex mtx("mutex", MutexFlag::create);
    MemoryMap<int> data("data", 3, ModeFlags::read);
    mtx.lock();     // waiting client
    mtx.lock();
    for (int i = 0; i < 3; ++i) {
        std::cout << data[i] << ' ';
    }
    std::cout << std::endl;
    mtx.unlock();
    mtx.delete_for_all();

    return 0;
}
