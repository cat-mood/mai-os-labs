#include <unistd.h>
#include <iostream>
#include <string>
#include <cctype>

int main() {
    int pipe1_fd[2], pipe2_fd[2], pipe3_fd[2];
    if (pipe(pipe1_fd) == -1) {
        perror("Pipe error!\n");
        return -1;
    }
    if (pipe(pipe2_fd) == -1) {
        perror("Pipe error!\n");
        return -1;
    }
    if (pipe(pipe3_fd) == -1) {
        perror("Pipe error!\n");
        return -1;
    }
    pid_t child1 = fork();
    if (child1 == -1) {
        perror("Fork error!\n");
        return -1;
    }
    pid_t child2 = fork();
    if (child2 == -1) {
        perror("Fork error!\n");
        return -1;
    }
    if (child1 == 0) {
        close(pipe1_fd[1]);
        close(pipe2_fd[0]);
        close(pipe3_fd[0]);
        close(pipe3_fd[1]);
        std::string s;
        int err_read = read(pipe1_fd[0], &s, sizeof(s));
        while (err_read != 0) {
            if (err_read == -1) {
                perror("Read error!");
                return -1;
            }
            for (char & c : s) {
                c = toupper(c);
            }
            if (write(pipe2_fd[1], &s, sizeof(s)) == -1) {
                perror("Write error!\n");
                return -1;
            }
            err_read = read(pipe1_fd[0], &s, sizeof(s));
        }
        close(pipe1_fd[0]);
        close(pipe2_fd[1]);
    } else if (child2 == 0) {
        close(pipe1_fd[0]);
        close(pipe1_fd[1]);
        close(pipe2_fd[1]);
        close(pipe3_fd[0]);
        std::string s;
        int err_read = read(pipe2_fd[0], &s, sizeof(s));
        while (err_read != 0) {
            if (err_read == -1) {
                perror("Read error!");
                return -1;
            }
            for (char & c : s) {
                if (c == ' ') {
                    c = '_';
                }
            }
            if (write(pipe3_fd[1], &s, sizeof(s)) == -1) {
                perror("Write error!\n");
                return -1;
            }
            err_read = read(pipe2_fd[0], &s, sizeof(s));
        }
        close(pipe3_fd[1]);
        close(pipe2_fd[0]);
    } else {
        close(pipe1_fd[0]);
        close(pipe2_fd[0]);
        close(pipe2_fd[1]);
        close(pipe3_fd[1]);
        std::string s;
        do {
            std::cout << "Input string: ";
            std::getline(std::cin, s);
            if (write(pipe1_fd[1], &s, sizeof(s)) == -1) {
                perror("Write error!\n");
                return -1;
            }
            if (read(pipe3_fd[0], &s, sizeof(s)) == -1) {
                perror("Read error!");
                return -1;
            }
            std::cout << "Result: " << s << std::endl;
        } while (s != "");
        close(pipe1_fd[1]);
        close(pipe3_fd[0]);
    }
    
    return 0;
}
