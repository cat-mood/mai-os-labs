#include <iostream>
#include "client.h"

using namespace bc;

void help() {
    std::cout << "1. Create a game\n";
    std::cout << "2. Connect to the game\n";
    std::cout << "q. Exit" << std::endl;
}

void game() {
    // there will be a game interface
    return;
}

void interface(Client& cl) {
    char cmd;
    std::cout << "Type <h> for help page" << std::endl;
    while (true) {
        std::cout << "> ";
        std::cin >> cmd;
        switch (cmd) {
            case '1': {
                std::string game_name;
                int players;
                std::cout << "Enter game name: ";
                std::cin >> game_name;
                std::cout << "Enter number of players: ";
                std::cin >> players;
                try {
                    cl.create_game(game_name, players);
                } catch (std::exception& ex) {
                    std::cout << "Error: " << ex.what() << std::endl;
                    break;
                }
                std::cout << "meow 1" << std::endl;
                cl.connect_to_game(game_name);
                std::cout << "meow 2" << std::endl;
                game();
                break;
            }
            case '2': {
                std::string game_name;
                std::cout << "Enter game name: ";
                std::cin >> game_name;
                cl.connect_to_game(game_name);
                game();
                break;
            }
            case 'q': {
                return;
            }
            case 'h': {
                help();
                break;
            }
            default: {
                std::cout << "Wrong command" << std::endl;
            }
        }
    }
}

int main() {
    std::cout << "Starting client" << std::endl;
    std::cout << "Enter server id: ";
    int server_id;
    std::cin >> server_id;
    Client cl(server_id);
    interface(cl);

    return 0;
}
