#pragma once

#include <zmq.hpp>
#include <unistd.h>
#include <vector>
#include <string>
#include <fstream>
#include "message_type.h"
#include "binary_tree.h"

namespace mysys {
    class ControlNode {
    public:
        ControlNode(int base_port = 5000);
        ControlNode(const ControlNode& other) = delete;
        ControlNode(ControlNode&& other) noexcept;
        ~ControlNode() noexcept;
        pid_t new_node(int id);
        std::vector<int> pingall();
        std::string exec(int id, const std::string& text, const std::string& pattern);
        MyMessage get_message(zmq::recv_flags flags = zmq::recv_flags::none);
        bool send_message(const MyMessage& msg);
    private:
        zmq::context_t _context;
        zmq::socket_t _s_request;
        int _base_port;
        BinaryTree<int> _topology;
        bool _has_child;

        std::vector<int> _string_to_vector(const std::string& str);
        void _msg_to_string(const zmq::message_t& msg, std::string& str);
    };
}
