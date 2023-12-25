#pragma once

#include <zmq.hpp>
#include <string>
#include <vector>
#include <map>
#include "message_type.h"

namespace mysys {
    class CalculatingNode {
    public:
        CalculatingNode(int id, int base_port);
        CalculatingNode(const CalculatingNode& other) = delete;
        CalculatingNode(CalculatingNode&& other) noexcept;
        ~CalculatingNode() noexcept;
        void connect_child(int child_id);
        std::vector<int> ping_children();      // returns id of unavailable child
        MyMessage get_child_msg(zmq::socket_t& child);
        MyMessage get_parent_msg();
        std::vector<int> exec(const std::string& text, const std::string& pattern);
        void req(zmq::socket_t& child, const MyMessage& msg);
        void reply(const MyMessage& msg);
    private:
        zmq::context_t _context;
        zmq::socket_t _s_parent;     // like server (to parent)
        std::map<int, zmq::socket_t*> _s_children;
        int _base_port;
        int _id;

        void _msg_to_string(const zmq::message_t& msg, std::string& str);
    };
}
