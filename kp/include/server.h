#pragma once

#include <string>
#include "mmap.h"

namespace bc {
    class Server {
    private:
        
    public:
        Server(std::string mmap_name);
        ~Server();
        void create_game(std::string game_name, int players);

    };
}   // bulls & cows
