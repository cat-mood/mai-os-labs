#include "lib_lab04.h"
#include <stdio.h>

void interface() {
    printf("Test program 1. \nFor help enter 'h' \n> ");
    while (1) {
        char cmd;
        cmd = getchar();
        if (cmd == 'h') {
            printf("h - display this page you are seeing now\n");
            printf("1 A B E - compute integral of sin(x) on the segment [A, B] with step E\n");
            printf("2 A B - compute square of rectangle size of A x B\n");
            printf("q - quit\n");
        } else if (cmd == '1') {
            float a, b, e;
            scanf(" %f %f %f", &a, &b, &e);
            printf("Result of sin_integral is %f\n", sin_integral(a, b, e));
        } else if (cmd == '2') {
            float a, b;
            scanf(" %f %f", &a, &b);
            printf("Result of square is %f\n", square(a, b));
        } else if (cmd == 'q'){
            break;
        } else {
            while (cmd != '\n') {
                getchar();
            }
            printf("Invalid command!\n");
        }
        printf("> ");
        cmd = getchar();
    }
}

int main() {
    interface();
    return 0;
}
