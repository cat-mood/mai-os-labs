#include "control_node.h"
#include <iostream>

using namespace mysys;

void ControlNode::_msg_to_string(const zmq::message_t& msg, std::string& str) {
    str.resize(msg.size() / sizeof(char));
    std::memcpy(str.data(), msg.data(), msg.size());
}

template<typename Item>
void concat(std::vector<Item> &a, std::vector<Item> &b) {
    a.reserve(a.size() + b.size());
    a.insert(
        a.end(),
        std::make_move_iterator(b.begin()),
        std::make_move_iterator(b.end())
    );
}

ControlNode::ControlNode(int base_port) : _base_port{base_port}, 
    _s_request(_context, zmq::socket_type::pair), _has_child{false} {
    _s_request.set(zmq::sockopt::sndtimeo, 3000);
}

ControlNode::ControlNode(ControlNode&& other) noexcept {
    _context = std::move(other._context);
    _s_request = std::move(other._s_request);
    _base_port = std::move(other._base_port);
    _topology = std::move(other._topology);
}

ControlNode::~ControlNode() noexcept {}

pid_t ControlNode::new_node(int id) {
    if (id == 0) throw std::logic_error("id 0 is reserved for server");
    _topology.insert(id);
    int parent = _topology.get_parent(id);
    pid_t pid = fork();
    if (pid == 0) {
        execl("./lab5-7_calc", "./lab5-7_calc", std::to_string(id).c_str(), std::to_string(_base_port).c_str());
    } else {
        if (!_has_child) {
            std::string addr = "tcp://*:" + std::to_string(_base_port + id);
            _s_request.bind(addr);
            _has_child = true;
        } else {
            MyMessage bind;
            bind.type = MessageType::bind_node;
            bind.text = std::to_string(parent) + " " + std::to_string(id);
            send_message(bind);
        }
        return pid;
    }
}

std::vector<int> ControlNode::_string_to_vector(const std::string& str) {
    std::stringstream ss(str);
    std::vector<int> vec;
    int num;
    while (ss >> num) {
        vec.push_back(num);
    }
    return vec;
}

MyMessage ControlNode::get_message(zmq::recv_flags flags) {
    MyMessage msg;
    std::string buf;
    zmq::message_t rec;
    auto res = _s_request.recv(rec, flags);
    _msg_to_string(rec, buf);
    msg.type = (MessageType) std::stoi(buf);
    res = _s_request.recv(rec);
    _msg_to_string(rec, buf);
    msg.text = buf;
    return msg;
}

bool ControlNode::send_message(const MyMessage& msg) {
    zmq::message_t msg_type(std::to_string(msg.type));
    if (msg.type == MessageType::ping) {
        auto res = _s_request.send(msg_type, zmq::send_flags::dontwait);
        if (!res) return false;
        return true;
    }
    _s_request.send(msg_type, zmq::send_flags::sndmore);
    zmq::message_t msg_text(msg.text);
    _s_request.send(msg_text, zmq::send_flags::none);
    return true;
}

std::vector<int> ControlNode::pingall() {
    MyMessage msg;
    msg.type = MessageType::ping;
    if (!send_message(msg)) {
        std::vector<int> ids = _topology.get_tops();
        return ids;
    }
    msg = get_message();
    std::vector<int> ids = _string_to_vector(msg.text);
    std::vector<int> tops;
    for (auto el : ids) {
        std::vector<int> children = _topology.get_children(el);
        concat<int>(tops, children);
    }
    return tops;
}

std::string ControlNode::exec(int id, const std::string& text, const std::string& pattern) {
    MyMessage msg;
    msg.type = MessageType::exec;
    msg.text = std::to_string(id) + ' ' + text + ' ' + pattern;
    send_message(msg);
    msg = get_message();
    if (msg.type != MessageType::exec_result) throw std::runtime_error("Wrong message type");
    return msg.text;
}
