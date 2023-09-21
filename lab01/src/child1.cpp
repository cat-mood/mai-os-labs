#include <iostream>
#include <unistd.h>

int main() {
    char c;
    while (read(STDIN_FILENO, &c, sizeof(c)) != -1) {
        c = toupper(c);
        write(STDOUT_FILENO, &c, sizeof(c));
    }
    close(0);
    close(1);

    return 0;
}
