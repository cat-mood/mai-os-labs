#include <iostream>
#include <sstream>
#include "client.h"

using namespace bc;

void help() {
    std::cout << "1. Create a game\n";
    std::cout << "2. Connect to the game\n";
    std::cout << "q. Exit" << std::endl;
}

void game(Client& cl) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    if (!cl.ping()) {
        std::cerr << "Can't connect to game\n";
        exit(1);
    }
    std::cout << "Waiting for start" << std::endl;
    Message msg;
    bool is_new_round_started = false;
    while (true) {
        msg = cl.get_message();
        if (msg.type == MessageType::start_round) {
            std::cout << "Round started" << std::endl;
            is_new_round_started = true;
        } else if (msg.type == MessageType::wait || msg.type == MessageType::guess) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        } else if (msg.type == MessageType::ans_guess) {
            std::istringstream iss(msg.data);
            int pid;
            iss >> pid;
            if (pid != cl.id()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            int cows, bulls;
            iss >> cows;
            std::cout << "cows " << cows << ": ";
            for (int i = 0; i < cows; ++i) {
                char letter;
                iss >> letter;
                std::cout << letter << ' ';
            }
            std::cout << '\n';
            iss >> bulls;
            std::cout << "bulls " << bulls << ": ";
            for (int i = 0; i < bulls; ++i) {
                char letter;
                iss >> letter;
                std::cout << letter << ' ';
            }
            std::cout << std::endl;
            cl.clear_mmap();
        } else if (msg.type == MessageType::end_game) {
            std::istringstream iss(msg.data);
            int player_id;
            iss >> player_id;
            if (player_id == cl.id()) {
                std::cout << "You win!" << std::endl;
            } else {
                std::cout << "Player " << player_id << " win." << std::endl;
            }
            return;
        }
        if (is_new_round_started) {
            is_new_round_started = false;
            std::cout << "Enter a word: ";
            std::string word;
            std::cin >> word;
            while (word.size() != 5) {
                std::cout << "Word lenght must be 5" << std::endl;
                std::cout << "Enter a word: ";
                std::cin >> word;
            }
            Message guess_msg;
            guess_msg.type = MessageType::guess;
            guess_msg.data = std::to_string(cl.id()) + word;
            msg = cl.get_message();
            while (msg.type != MessageType::start_round && msg.type != MessageType::wait) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                msg = cl.get_message();
            }
            if (msg.type == MessageType::end_game) {
                std::istringstream iss(msg.data);
                int player_id;
                iss >> player_id;
                if (player_id == cl.id()) {
                    std::cout << "You win!" << std::endl;
                } else {
                    std::cout << "Player " << player_id << " win." << std::endl;
                }
            }
            cl.send_message(guess_msg);
        }
    }
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
                cl.connect_to_game(game_name);
                game(cl);
                cl.connect_to_server();
                break;
            }
            case '2': {
                std::string game_name;
                std::cout << "Enter game name: ";
                std::cin >> game_name;
                cl.connect_to_game(game_name);
                game(cl);
                cl.connect_to_server();
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
                break;
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
    std::cout << "Your client id: " << cl.id() << std::endl;
    if (!cl.ping()) {
        std::cerr << "Can't connect to server\n";
        return 1;
    }
    interface(cl);

    return 0;
}
