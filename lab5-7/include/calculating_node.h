#pragma once

#include <zmq.hpp>
#include <string>
#include <vector>
#include <map>
#include <array>
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
        MyMessage get_child_msg(zmq::socket_t* child);
        MyMessage get_parent_msg();
        bool req(zmq::socket_t* child, const MyMessage& msg);
        void reply(const MyMessage& msg);
        int id() const;
        std::array<std::pair<int, zmq::socket_t*>, 2> children() const;
        std::vector<int> _string_to_vector(const std::string& str);
        std::string exec(const std::string& text, const std::string& pattern);
        zmq::socket_t* get_less_child() const;
        zmq::socket_t* get_greater_child() const;
    private:
        zmq::context_t _context;
        zmq::socket_t _s_parent;     // like server (to parent)
        std::array<std::pair<int, zmq::socket_t*>, 2> _s_children;
        int _base_port;
        int _id;

        void _msg_to_string(const zmq::message_t& msg, std::string& str);
    };
}
