#include "calculating_node.h"
#include <fstream>

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
        }
    }

    return 0;
}
