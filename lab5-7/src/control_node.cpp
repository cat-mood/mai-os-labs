#include "control_node.h"

using namespace mysys;

ControlNode::ControlNode(int base_port) : _base_port{base_port}, 
    _s_reply(_context, zmq::socket_type::rep) {
    std::string addr = "tcp://*:" + std::to_string(_base_port);
    _s_reply.bind(addr);
}

ControlNode::ControlNode(const ControlNode& other) : 
    ControlNode(other._base_port) {
    _topology = other._topology;
}

ControlNode::ControlNode(ControlNode&& other) noexcept {
    _context = std::move(other._context);
    _s_reply = std::move(other._s_reply);
    _base_port = std::move(other._base_port);
    _topology = std::move(other._topology);
}

ControlNode::~ControlNode() noexcept {}

void ControlNode::new_node(int id) {
    if (id == 0) throw std::logic_error("id 0 is reserved for server");
    int parent_id = _topology.insert(id);
    // there will be a fork
}
