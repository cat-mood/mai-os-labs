#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int impl;
    void* handles[2];
    void* funcs[2];
} lib;

void change_implementation(lib* l, int impl) {
    l->funcs[0] = dlsym(l->handles[impl], "sin_integral");
    l->funcs[1] = dlsym(l->handles[impl], "square");
    l->impl = impl;
}

void interface(lib* l) {
    printf("Test program 1. \nFor help enter 'h' \n> ");
    while (1) {
        char cmd;
        cmd = getchar();
        if (cmd == 'h') {
            printf("h - display this page you are seeing now\n");
            printf("0 - change implementation (could be 0 or 1)");
            printf("1 A B E - compute integral of sin(x) on the segment [A, B] with step E\n");
            printf("2 A B - compute square of rectangle size of A x B\n");
            printf("q - quit\n");
        } else if (cmd == '0') {
            change_implementation(l, (l->impl + 1) % 2);
            printf("Implementation changed. Current is %d\n", l->impl);
        } else if (cmd == '1') {
            float a, b, e;
            scanf(" %f %f %f", &a, &b, &e);
            printf("Result of sin_integral is %f\n", (((float (*)(float, float, float)) l->funcs[0])(a, b, e)));
        } else if (cmd == '2') {
            float a, b;
            scanf(" %f %f", &a, &b);
            printf("Result of square is %f\n", ((float (*)(float, float)) l->funcs[1])(a, b));
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
    lib l;
    l.handles[0] = dlopen("/home/cat_mood/programming/mai-os-labs/lab04/build/liblib1.so", RTLD_LAZY | RTLD_LOCAL);
    if (l.handles[0] == NULL) {
        exit(1);
    }
    l.handles[1] = dlopen("/home/cat_mood/programming/mai-os-labs/lab04/build/liblib2.so", RTLD_LAZY | RTLD_LOCAL);
    if (l.handles[1] == NULL) {
        exit(1);
    }
    change_implementation(&l, 0);

    interface(&l);

    dlclose(l.handles[0]);
    dlclose(l.handles[1]);

    return 0;
}
