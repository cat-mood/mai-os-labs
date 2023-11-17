#include "mmap.h"
#include <sys/wait.h>

int create_process() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork error!\n");
        exit(-1);
    }
    return pid;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        perror("Too few arguments. Usage: ./lab03 NAME_OF_FILE");
        exit(1);
    }
    std::string mm_name(argv[1]);
    MemoryMap<char> mm(mm_name, sizeof(char)*MAX_LENGHT, M_WRITE | M_READ);
    char c = getchar();
    int i = 1;
    while (c != EOF) {
        mm[i] = c;
        ++i;
        if (i == MAX_LENGHT) {
            break;
        }
        c = getchar();
    }
    mm[0] = i - 1;    // number of elements
    int pid = create_process();
    if (pid == 0) {     // child 1
        execl("../build/child1", "../build/child1",  mm_name.c_str(), NULL);
    } else {    // parent
        wait(NULL);
        int elems = mm[0];
        for (int i = 1; i <= elems; ++i) {
            putchar(mm[i]);
        }
        mm.delete_shm_file();
    }

    return 0;
}
