// memcheck_extra_cases.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>


// Double free
void case_double_free() {
    int *c = malloc(4 * sizeof(int));
    free(c);
    free(c);             // Double free
}

// Free invalid pointer
void case_free_invalid_pointer() {
    int *arr = malloc(10 * sizeof(int));
    int *mid = arr + 2;
    free(mid); // mid không phải là pointer gốc
    free(arr); // Đúng
}

int main() {
    case_double_free();
    case_free_invalid_pointer();
    return 0;
}
