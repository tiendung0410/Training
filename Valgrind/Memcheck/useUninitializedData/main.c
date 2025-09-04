// memcheck_extra_cases.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>


// Use of uninitialized value
void case_uninitialized_value() {
    int *e = malloc(2 * sizeof(int));
    if(e[0])
    {
        e[1]++;
    }
    free(e);
}

// 10. Use of uninitialized/unaddressable value in system call
void case_uninit_syscall() {
    char buf[8];
    // buf is uninitialized!
    write(1, buf, 8);  
}

int main() {
    printf("Test Memcheck all cases:\n");
    case_uninitialized_value();
    case_uninit_syscall();

    printf("\nDone!\n");
    return 0;
}
