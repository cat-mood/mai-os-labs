#pragma once

#include <unistd.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <exception>
#include <string>

namespace bc {
    enum ModeFlags {
        read = PROT_READ,
        write = PROT_WRITE,
        exec = PROT_EXEC,
        none = PROT_NONE
    };

    template <class T>
    class MemoryMap {
    public:
        MemoryMap() = delete;
        MemoryMap(const std::string& s, size_t size, int mode);
        void delete_shm_file();
        ~MemoryMap();
        T* data() const noexcept;
        size_t size() const noexcept;
        T& operator[](int idx);
        const T& operator[](int idx) const;
    private:
        T* _data;
        int _fd;
        std::string _name;
        size_t _size;
    };

    template <class T>
    MemoryMap<T>::MemoryMap(const std::string& name, size_t size, int mode) : _name{name}, _size{size} {
        _fd = shm_open(name.c_str(), O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
        if (ftruncate(_fd, sizeof(T) * size) != 0) {
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
    T* MemoryMap<T>::data() const noexcept {
        return _data;
    }

    template <class T>
    size_t MemoryMap<T>::size() const noexcept {
        return _size;
    }

    template <class T>
    T& MemoryMap<T>::operator[](int idx) {
        if (idx > _size - 1) {
            throw std::range_error("out of range");
        }
        return _data[idx];
    }

    template <class T>
    const T& MemoryMap<T>::operator[](int idx) const {
        if (idx > _size - 1) {
            throw std::range_error("out of range");
        }
        return _data[idx];
    }

    template <class T>
    void str_to_mmap(const std::string& str, MemoryMap<T>& mmap, int start_idx) {
        if (mmap.size() - start_idx < str.size()) throw std::logic_error("string is too long");
        for (int i = start_idx; i < str.size(); ++i) {
            mmap[i] = str[i];
        }
        mmap[start_idx + str.size()] = '\0';
    }

    template <class T>
    std::string mmap_to_str(const MemoryMap<T>& mmap, int start_idx) {
        std::string str(mmap.size() - start_idx, ' ');
        int i = start_idx;
        while (mmap[i] != '\0') {
            str[i] = mmap[i];
            ++i;
        }
        return str;
    }
}   // bulls & cows
