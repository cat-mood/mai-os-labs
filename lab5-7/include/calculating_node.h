#pragma once

#include <zmq.hpp>
#include <string>
#include <vector>

namespace mysys {
    class CalculatingNode {
    public:
        CalculatingNode(int parent_id);
        CalculatingNode(const CalculatingNode& other);
        CalculatingNode(CalculatingNode&& other) noexcept;
        ~CalculatingNode() noexcept;
        void req(const std::string& request);
        void reply();
    private:
        zmq::context_t _context;
        zmq::socket_t _s_reply;
        zmq::socket_t _s_request;
        int _base_port;
        int _parent_id;
        int _child_id;

        std::vector<int> _exec(const std::string& text, const std::string& pattern);
    };
}
