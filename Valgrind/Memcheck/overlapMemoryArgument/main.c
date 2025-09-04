// memcheck_extra_cases.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>



// Overlapping memcpy
void case_overlap_memcpy() {
    char buf[8] = "abcdefg";
    strcpy(buf, buf+1); // Overlap
}


int main() {
    case_overlap_memcpy();
    return 0;
}
