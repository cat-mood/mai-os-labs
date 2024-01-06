#pragma once

#include <unistd.h>
#include <string>
#include "mmap.h"
#include "mutex.h"
#include "message.h"

namespace bc {
    class Client {
    private:
        int _id;
        MemoryMap<char> _mmap;
        Mutex _mtx;
        static const int _mmap_size = 10000;
    public:
        Client(int server_id);
        ~Client();
        int id() const;
        void create_game(const std::string& game_name, int players);
        void connect_to_game(const std::string& game_name);
        void send_message(const Message& msg);
        Message get_message();
    };
}   // bulls & cows
