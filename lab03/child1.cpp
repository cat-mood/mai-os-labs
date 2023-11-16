#include "mmap.h"
#include <sys/wait.h>
#include <unistd.h>

int create_process() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork error!\n");
        exit(-1);
    }
    return pid;
}

int main(int argc, char** argv) {
    std::string mm_name(argv[1]);
    MemoryMap<char> mm(mm_name, sizeof(char) * MAX_LENGHT, M_WRITE | M_READ);
    int elems = ((char *) mm.data())[0];
    for (int i = 1; i <= elems; ++i) {
        mm[i] = toupper(mm[i]);
    }
    int pid = create_process();
    if (pid == 0) {     // child 2
        execl("../build/child2", "../build/child2", argv[1], NULL);
    } else {    // child 1
        wait(NULL);
    }

    return 0;
}
