#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include "server.h"

using namespace bc;

void routine(Server& ser) {
    Message msg;
    while (true) {
        msg = ser.get_message();
        switch (msg.type) {
            case MessageType::wait: {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                break;
            }
            case MessageType::server_create_game: {
                std::cout << "meow create" << std::endl;
                std::istringstream oss(msg.data);
                std::string game_name;
                int players;
                oss >> game_name >> players;
                try {
                    ser.create_game(game_name, players);
                } catch (std::exception& ex) {
                    std::cout << "Error: " << ex.what() << std::endl;
                }
                ser.clear_mmap();
                std::cout << "meow clear" << std::endl;
                break;
            }
        }
    }
}

int main() {
    std::cout << "Starting server" << std::endl;
    Server ser;
    std::cout << "Server id: " << ser.id() << std::endl;
    routine(ser);

    return 0;
}
