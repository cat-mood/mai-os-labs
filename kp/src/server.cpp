#include "server.h"

using namespace bc;

Server::Server() : _id{getpid()}, 
_mmap("bc_" + std::to_string(_id) + "_mmap", _mmap_size, ModeFlags::read | ModeFlags::write),
_mtx("bc_" + std::to_string(_id) + "_mutex", MutexFlag::create) {
    clear_mmap();
}

Server::~Server() {
    _mtx.delete_for_all();
    _mmap.delete_shm_file();
}

void Server::create_game(const std::string& game_name, int players) {
    int pid = fork();
    if (pid == -1) throw std::runtime_error("fork error");
    if (pid == 0) {
        int er = execl("../build/kp_game", "../build/kp_game", game_name.c_str(), std::to_string(players).c_str(), NULL);
        if (er == -1) {
            std::cout << "create er: " << errno << std::endl;
            exit(1);
        }
    }
}

int Server::id() const {
    return _id;
}

void Server::send_message(const Message& msg) {
    _mtx.lock();
    _mmap[0] = msg.type;
    str_to_mmap(msg.data, _mmap, 1);
    _mtx.unlock();
}

Message Server::get_message() {
    Message msg;
    _mtx.lock();
    msg.type = (MessageType) _mmap[0];
    if (msg.type == MessageType::wait) {
        _mtx.unlock();
        return msg;
    } 
    msg.data = mmap_to_str(_mmap, 1);
    _mtx.unlock();
    return msg;
}

void Server::clear_mmap() {
    _mtx.lock();
    _mmap[0] = MessageType::wait;
    _mtx.unlock();
}
