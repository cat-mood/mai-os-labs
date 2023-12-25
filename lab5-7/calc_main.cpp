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
    std::string s;
    s += "log";
    s += argv[1];
    s += ".txt";
    std::ofstream file(s);
    file << "test\n";
    while (true) {
        MyMessage msg = node.get_parent_msg();
        if (msg.type == MessageType::ping) {
            std::vector<int> ids = node.ping_children();
            for (auto el : ids) {
                file << el << ' ';
            }
            // file << '\n' << "size = " << ids.size() << " okey\n";
            // file.close();
            MyMessage rep;
            rep.type = MessageType::ping_result;
            rep.text = vector_to_string(ids);
            node.reply(rep);
        } else if (msg.type == MessageType::bind_node) {
            file << "connect\n";
            file.close();
            node.connect_child(std::stoi(msg.text));
        }
    }

    return 0;
}
