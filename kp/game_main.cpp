#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>
#include "game.h"

using namespace bc;

const std::string DB_NAME = "../db/test_db.bin";

void routine(Game& game) {
    int players = 0;
    int attempts = 0;
    Message msg;
    bool is_game_ended = false;
    while (!is_game_ended) {
        msg = game.get_message();
        switch (msg.type) {
            case MessageType::wait: {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                ++attempts;
                if (attempts > 30) --game.players();
                if (players == game.players()) {
                    Message call;
                    call.type = MessageType::start_round;
                    call.data = "";
                    game.send_message(call);
                    players = 0;
                    attempts = 0;
                }
                break;
            }
            case MessageType::ping: {
                attempts = 0;
                ++players;
                game.clear_mmap();
                break;
            }
            case MessageType::guess: {
                attempts = 0;
                ++players;
                std::istringstream iss(msg.data);
                int pid;
                std::string word;
                iss >> pid >> word;
                std::string ans = game.check_word(word, pid);
                if (game.winner_id() != 0) {
                    is_game_ended = true;
                }
                msg.type = MessageType::ans_guess;
                msg.data = std::to_string(pid) + " " + ans;
                game.send_message(msg);
                break;
            }
            default:
                attempts = 0;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                break;
        }
    }
    msg.type = MessageType::end_game;
    msg.data = std::to_string(game.winner_id());
    game.send_message(msg);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Wrong number of args\n";
        return 1;
    }
    std::string filename(argv[1]);
    filename += "_log.txt";
    std::ofstream ofs(filename);
    ofs << "Game started.\n";
    int players = std::stoi(argv[2]);
    Game game(argv[1], players, DB_NAME);
    ofs << "Word: " << game.word() << '\n';
    routine(game);
    ofs << "Winner id: " << game.winner_id() << '\n';
    ofs << "Game ended.\n";
    ofs.close();

    return 0;
}
