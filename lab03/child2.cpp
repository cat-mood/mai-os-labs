#include "mmap.h"
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv) {
    std::string mm_name(argv[1]);
    MemoryMap<char> mm(mm_name, sizeof(char) * MAX_LENGHT, M_WRITE | M_READ);
    int elems = mm[0];
    for (int i = 1; i <= elems; ++i) {
        if (mm[i] == ' ') {
            mm[i] = '_';
        }
    }

    return 0;
}
