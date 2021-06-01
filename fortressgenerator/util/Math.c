#include "Math.h"

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int abs(int a) {
    return a < 0 ? -a : a;
}