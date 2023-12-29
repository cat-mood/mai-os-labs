#pragma once

#include "mmap.h"

namespace bc {
    class Game {
    private:
        MemoryMap<int> _mmap;
        std::string _name;
        int _id;
        std::string _word;

        std::string _generate_word();
    public:
        Game(const std::string& name, int id);
        Game(const Game& other) = delete;
        ~Game();
        void reply();
    };
}   // bulls & cows
