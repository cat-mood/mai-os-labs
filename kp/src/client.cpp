#include "client.h"

using namespace bc;

Client::Client(int server_id) : _id{getpid()}, _server_id{server_id}, 
_mmap("bc_" + std::to_string(server_id) + "_mmap", _mmap_size, ModeFlags::read | ModeFlags::write),
_mtx("bc_" + std::to_string(server_id) + "_mutex", MutexFlag::connect) {}

Client::~Client() {}

int Client::id() const {
    return _id;
}

void Client::create_game(const std::string& game_name, int players) {
    // 4 = players_num + ' ' + state + end of str
    if (game_name.size() > _mmap_size - 4) throw std::logic_error("Too long game name");
    Message msg;
    msg.type = MessageType::server_create_game;
    msg.data = game_name + ' ' + std::to_string(players) + '\0';
    send_message(msg);
}

void Client::connect_to_game(const std::string& game_name) {
    MemoryMap<char> mmap("bcgame_" + game_name + "_mmap", _mmap_size, ModeFlags::read | ModeFlags::write);
    _mmap = mmap;
    Mutex mtx("bcgame_" + game_name + "_mutex", MutexFlag::connect);
    _mtx = mtx;
}

void Client::connect_to_server() {
    MemoryMap<char> mmap("bc_" + std::to_string(_server_id) + "_mmap", _mmap_size, ModeFlags::read | ModeFlags::write);
    _mmap = mmap;
    Mutex mtx("bc_" + std::to_string(_server_id) + "_mutex", MutexFlag::connect);
    _mtx = mtx;
}

void Client::send_message(const Message& msg) {
    _mtx.lock();
    _mmap[0] = msg.type;
    str_to_mmap(msg.data, _mmap, 1);
    _mtx.unlock();
}

Message Client::get_message() {
    Message msg;
    _mtx.lock();
    msg.type = (MessageType) _mmap[0];
    msg.data = mmap_to_str(_mmap, 1);
    _mtx.unlock();
    return msg;
}

bool Client::ping() {
    Message msg;
    msg.type = MessageType::ping;
    msg.data = "ping";
    send_message(msg);
    msg = get_message();
    int attempts = 0;
    while (msg.type == MessageType::ping) {
        ++attempts;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        msg = get_message();
        if (attempts == 20) break;
    }
    if (msg.type == MessageType::wait || msg.type == MessageType::start_round) return true;
    return false;
}

void Client::clear_mmap() {
    _mtx.lock();
    _mmap[0] = MessageType::wait;
    _mmap[1] = '\0';
    _mtx.unlock();
}
