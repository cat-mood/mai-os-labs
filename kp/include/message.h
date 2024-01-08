#pragma once

#include <string>

namespace bc {
    enum MessageType {
        wait,
        server_create_game,
        ping,
        start_round,
        end_game,
        guess,
        ans_guess
    };

    struct Message {
        MessageType type;
        std::string data;
    };
}   // bulls & cows
