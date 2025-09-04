// memcheck_extra_cases.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

// 1. Invalid read/write (out-of-bounds)
void case_invalid_rw() {
    int *a = malloc(4 * sizeof(int));
    a[4] = 1;            // Invalid write
    int x = a[5];        // Invalid read
    free(a);
}

// 2. Use after free
void case_use_after_free() {
    int *b = malloc(4 * sizeof(int));
    free(b);
    b[0] = 2;            // Use after free
}



int main() {
    case_invalid_rw();
    case_use_after_free();
    return 0;
}
