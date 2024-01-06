#pragma once

#include <string>

namespace bc {
    enum MessageType {
        wait,
        server_create_game,
        game_connect,
        game_check
    };

    struct Message {
        MessageType type;
        std::string data;
    };
}   // bulls & cows
