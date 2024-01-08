#pragma once

#include <set>
#include <fstream>
#include <random>
#include "mmap.h"
#include "mutex.h"
#include "message.h"

namespace bc {
    class Game {
    private:
        MemoryMap<char> _mmap;
        Mutex _mtx;
        std::string _name;
        int _players;
        std::string _word;
        int _words;  // count of
        std::set<char> _letters;
        std::ifstream _db;
        int _winner_id;
        static const int _mmap_size = 10000;
    public:
        Game(const std::string& name, int players, const std::string& db_name);
        Game(const Game& other) = delete;
        ~Game() noexcept;
        void send_message(const Message& msg);
        Message get_message();
        void clear_mmap();
        const std::string& word() const;
        int players() const;
        int& players();
        const std::string& name() const;
        MemoryMap<char>& mmap();
        Mutex& mtx();
        int winner_id() const;
        std::string check_word(const std::string& word, int player_id);
    private:
        std::string _generate_word();
    };
}   // bulls & cows
