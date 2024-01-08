#include "mutex.h"

using namespace bc;

Mutex::Mutex(const std::string& name, MutexFlag flag) : _name{name}, _mtx(name, 1, ModeFlags::write | ModeFlags::read) {
    if (flag == MutexFlag::create) {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&_mtx[0], &attr);
    }
}

Mutex::Mutex(const Mutex& other) : _name{other._name}, _mtx{other._mtx} {}

Mutex::Mutex(Mutex&& other) noexcept : _name{std::move(other._name)}, _mtx{std::move(other._mtx)} {}

Mutex::~Mutex() noexcept {
    // there is no memory leak because of destructor of bc::MemoryMap
}

Mutex& Mutex::operator=(const Mutex& other) {
    _name = other._name;
    _mtx = other._mtx;
    return *this;
}

Mutex& Mutex::operator=(Mutex&& other) noexcept {
    _name = std::move(other._name);
    _mtx = std::move(other._mtx);
    return *this;
}

void Mutex::lock() {
    int res = pthread_mutex_lock(&_mtx[0]);
    if (res != 0) throw std::runtime_error(std::to_string(res));
}

void Mutex::unlock() {
    int res = pthread_mutex_unlock(&_mtx[0]);
    if (res != 0) throw std::runtime_error(std::to_string(res));
}

const std::string& Mutex::name() const {
    return _name;
}

void Mutex::delete_for_all() {
    _mtx.delete_shm_file();
}
