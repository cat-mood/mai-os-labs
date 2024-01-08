#include "game.h"

using namespace bc;

Game::Game(const std::string& name, int players, const std::string& db_name) : _name{name}, 
_mmap{"bcgame_" + name + "_mmap", _mmap_size, ModeFlags::read | ModeFlags::write},
_mtx{"bcgame_" + name + "_mutex", MutexFlag::create},
_db{db_name, std::ios::binary | std::ios::in},
_players{players},
_winner_id{0} {
    if (!_db.good()) throw std::runtime_error("Can't open database file");
    for (char letter : _word) {
        _letters.insert(letter);
    }
    _db.read(reinterpret_cast<char*>(&_words), sizeof(int));
    _word = _generate_word();
}

Game::~Game() noexcept {
    _db.close();
    _mmap.delete_shm_file();
    _mtx.delete_for_all();
}

void Game::send_message(const Message& msg) {
    _mtx.lock();
    _mmap[0] = msg.type;
    str_to_mmap(msg.data, _mmap, 1);
    _mtx.unlock();
}

Message Game::get_message() {
    Message msg;
    _mtx.lock();
    msg.type = (MessageType) _mmap[0];
    msg.data = mmap_to_str(_mmap, 1);
    _mtx.unlock();
    return msg;
}

std::string Game::_generate_word() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, _words - 1);
    _db.seekg(sizeof(int) + sizeof(char) * 5 * dist(rng), std::ios::beg);
    char raw_word[5];
    _db.read(raw_word, sizeof(char) * 5);
    std::string word(raw_word);
    return word; 
}

const std::string& Game::word() const {
    return _word;
}

void Game::clear_mmap() {
    _mtx.lock();
    _mmap[0] = MessageType::wait;
    _mmap[1] = '\0';
    _mtx.unlock();
}

int Game::players() const {
    return _players;
}

int& Game::players() {
    return _players;
}

const std::string& Game::name() const {
    return _name;
}

MemoryMap<char>& Game::mmap() {
    return _mmap;
}

Mutex& Game::mtx() {
    return _mtx;
}

int Game::winner_id() const {
    return _winner_id;
}

std::string Game::check_word(const std::string& word, int player_id) {
    int cows = 0;
    std::string str_cows = "";
    int bulls = 0;
    std::string str_bulls = "";
    for (int i = 0; i < word.size(); ++i) {
        if (word[i] == _word[i]) {
            ++bulls;
            str_bulls += word[i];
            str_bulls += ' ';
        } else if (_letters.find(word[i]) != _letters.end()) {
            ++cows;
            str_cows += word[i];
            str_cows += ' ';
        }
    }
    if (bulls == _word.size()) {
        _winner_id = player_id;
    }
    std::string ans = std::to_string(cows) + ' ' + str_cows + ' ' + std::to_string(bulls) + str_bulls;
    return ans;
}
