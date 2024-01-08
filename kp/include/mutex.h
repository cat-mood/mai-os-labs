#pragma once

#include <pthread.h>
#include <string>
#include "mmap.h"

/* This is my process-shared mutex */

// maybe i should add robust for ping server/client in future (i'm not sure that it's working as i think)

namespace bc {
    enum MutexFlag {
        create,
        connect
    };

    class Mutex {
    private:
        std::string _name;
        MemoryMap<pthread_mutex_t> _mtx;
    public:
        Mutex(const std::string& name, MutexFlag flag);
        Mutex(const Mutex& other);
        Mutex(Mutex&& other) noexcept;
        ~Mutex() noexcept;
        Mutex& operator=(const Mutex& other);
        Mutex& operator=(Mutex&& other) noexcept;
        void lock();
        void unlock();
        const std::string& name() const;
        void delete_for_all();   // it delete shmfile, should be called by server
    };
}   // bulls & cows
