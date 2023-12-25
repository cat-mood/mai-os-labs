#include <iostream>
#include <sstream>
#include "control_node.h"

using namespace mysys;

std::vector<int> string_to_vector(const std::string& str) {
    std::stringstream ss(str);
    std::vector<int> vec;
    int num;
    while (ss >> num) {
        vec.push_back(num);
    }
    return vec;
}

int main() {
    ControlNode ctrl;
    std::cout << "Starting Control Node" << std::endl;
    std::string cmd;
    while(true) {
        std::cout << "> ";
        std::cin >> cmd;
        if (cmd == "create") {
            int id;
            std::cin >> id;
            pid_t pid;
            try {
                pid = ctrl.new_node(id);
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                continue;
            }
            std::cout << "Ok: " << pid << std::endl;
        } else if (cmd == "pingall") {
            std::vector<int> ids = ctrl.pingall();
            std::cout << "Ok: ";
            if (ids.size() == 0) {
                std::cout << "-1";
            }
            for (auto id : ids) {
                std::cout << id << ";";
            }
            std::cout << std::endl;
        } else if (cmd == "q") {
            MyMessage msg;
            msg.type = MessageType::shutdown;
            ctrl.send_message(msg);
            break;
        } else if (cmd == "exec") {
            int id;
            std::cin >> id;
            std::string text_str, pattern_str;
            std::cin >> text_str >> pattern_str;
            std::string res = ctrl.exec(id, text_str, pattern_str);
            std::cout << res << std::endl;
        } else {
            std::cout << "Wrong command!" << std::endl;
        }
    }

    return 0;
}
