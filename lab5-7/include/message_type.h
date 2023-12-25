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
        error,
        shutdown
    };

    struct MyMessage {
        MessageType type;
        std::string text;
    };
}
