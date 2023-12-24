#pragma once

#include <zmq.hpp>
#include <unistd.h>
#include <vector>
#include <string>
#include "binary_tree.h"

namespace mysys {
    class ControlNode {
    public:
        ControlNode(int base_port = 5000);
        ControlNode(const ControlNode& other);
        ControlNode(ControlNode&& other) noexcept;
        ~ControlNode() noexcept;
        void new_node(int id);
        void pingall() const;
        void exec(int id, const std::string& text, const std::string& pattern) const;
    private:
        zmq::context_t _context;
        zmq::socket_t _s_reply;
        int _base_port;
        BinaryTree<int> _topology;

        std::vector<int> _pingall() const;
    };
}
