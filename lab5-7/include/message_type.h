#pragma once

#include <string>
#include <zmq.hpp>

namespace mysys {
    enum MessageType {
        exec,
        ping,
        bind_node,
        exec_result,
        ping_result,
        error
    };

    struct MyMessage {
        MessageType type;
        std::string text;
    };

    // void _msg_to_string(const zmq::message_t& msg, std::string& str) {
	//     str.resize(msg.size() / sizeof(char));
    //     std::memcpy(str.data(), msg.data(), msg.size());
    // }
}
