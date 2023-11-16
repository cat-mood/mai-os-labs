#pragma once

#include <unistd.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <exception>
#include <string>

#define M_READ PROT_READ
#define M_WRITE PROT_WRITE
#define M_EXEC PROT_EXEC
#define M_NONE PROT_NONE

const int MAX_LENGHT = 10000;

template <class T>
class MemoryMap {
public:
    MemoryMap() = delete;
    MemoryMap(const std::string& s, size_t size, int mode);
    void delete_shm_file();
    ~MemoryMap();
    T* data() noexcept;
    size_t size() noexcept;
    T& operator[](int idx);
private:
    T* _data;
    int _fd;
    std::string _name;
    size_t _size;
};

template <class T>
MemoryMap<T>::MemoryMap(const std::string& name, size_t size, int mode) : _name{name}, _size{size} {
    _fd = shm_open(name.c_str(), O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
    if (ftruncate(_fd, size) != 0) {
        throw std::runtime_error("ftruncate error");
    }
    if (_fd == -1) {
        throw std::runtime_error("shm_open error");
    }
    _data = (T*) mmap(NULL, size, mode, MAP_SHARED, _fd, 0);
    if (_data == MAP_FAILED) {
        throw std::runtime_error("mmap error");
    }
}

template <class T>
void MemoryMap<T>::delete_shm_file() {
    int error_code = shm_unlink(_name.c_str());
    if (error_code == -1) {
        throw std::runtime_error("shm_unlink");
    }
}

template <class T>
MemoryMap<T>::~MemoryMap() {
    munmap(_data, _size);
}

template <class T>
T* MemoryMap<T>::data() noexcept {
    return _data;
}

template <class T>
size_t MemoryMap<T>::size() noexcept {
    return _size;
}

template <class T>
T& MemoryMap<T>::operator[](int idx) {
    if (idx > _size - 1) {
        throw std::range_error("out of range");
    }
    return _data[idx];
}
