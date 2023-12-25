#include "calculating_node.h"

using namespace mysys;

std::string vector_to_string(const std::vector<int>& v) {
    std::string str = "";
    for (auto el : v) {
        str += std::to_string(el) + ' ';
    }
    return str;
}

int main(int argc, char** argv) {
    CalculatingNode node(std::stoi(argv[1]), std::stoi(argv[2]));
    while (true) {
        MyMessage msg = node.get_parent_msg();
        if (msg.type == MessageType::ping) {
            std::vector<int> ids = node.ping_children();
            MyMessage rep;
            rep.type = MessageType::ping_result;
            rep.text = vector_to_string(ids);
            node.reply(rep);
        } else if (msg.type == MessageType::bind_node) {
            int parent, id;
            std::stringstream ss(msg.text);
            ss >> parent >> id;
            if (parent == node.id()) {
                node.connect_child(id);
            } else if (parent < node.id()) {
                node.req(node.children()[0].second, msg);
            } else {
                node.req(node.children()[1].second, msg);
            }
        } else if (msg.type == MessageType::exec) {
            int id;
            std::stringstream ss(msg.text);
            ss >> id;
            if (id == node.id()) {
                std::string text, pattern;
                ss >> text >> pattern;
                MyMessage reply;
                reply.text = node.exec(text, pattern);
                reply.type = MessageType::exec_result;
                node.reply(reply);
            } else {
                MyMessage next;
                next.type = MessageType::exec;
                next.text = msg.text;
                if (id < node.id()) {
                    node.req(node.get_less_child(), next);
                    next = node.get_child_msg(node.get_less_child());
                } else {
                    node.req(node.get_greater_child(), next);
                    next = node.get_child_msg(node.get_greater_child());
                } 
                node.reply(next);
            }
        } else if (msg.type == MessageType::exec_result) {
            MyMessage next;
            next.type = MessageType::exec_result;
            next.text = msg.text;
            node.reply(next);
        }
    }

    return 0;
}
