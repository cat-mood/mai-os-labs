#include "lib_lab04.h"
#include <math.h>

float sin_integral(float a, float b, float e) {
    float res = 0;
    for (float i = a; i < b; i += e) {
        res += sinf(i);
    }
    res *= e;
    return res;
}

float square(float a, float b) {
    return a * b;
}
