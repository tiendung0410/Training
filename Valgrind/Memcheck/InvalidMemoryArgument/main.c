// memcheck_extra_cases.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>



// Invalid memory argument: malloc(0), calloc(0, n)
void case_fishy_arguments() {
    int x=-100;
    void *x1 = malloc(x);
    free(x1);
}

void case_realloc_size_zero() {
    void *p = malloc(4);
    p = realloc(p, 0);
    free(p);
}

int main() {
    case_fishy_arguments();
    case_realloc_size_zero();
    return 0;
}
