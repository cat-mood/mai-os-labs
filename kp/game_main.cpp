#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    std::ofstream ofs("log.txt");
    if (argc != 3) {
        ofs << "Wrong number of args";
        return 1;
    }
    ofs << "Game started.\n";
    ofs << "Game ended.\n";
    ofs.close();

    return 0;
}
