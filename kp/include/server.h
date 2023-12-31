#pragma once

#include <string>
#include <unistd.h>
#include "mmap.h"
#include "mutex.h"
#include "message.h"

namespace bc {
    class Server {
    private:
        int _id;
        MemoryMap<char> _mmap;
        Mutex _mtx;
        static const int _mmap_size = 10000;
    public:
        Server();
        ~Server();
        void create_game(const std::string& game_name, int players);
        int id() const;
        void send_message(const Message& msg);
        Message get_message();
        void clear_mmap();
    };
}   // bulls & cows
