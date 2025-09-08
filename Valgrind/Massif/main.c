// test_massif.c - Demo heap profiler với Massif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N_BUF 10
#define BUF_SIZE 1024 * 100   // 100KB/buffer

void foo() {
    char *a = malloc(500 * 1024); // 500KB
    memset(a, 'A', 500 * 1024);
    sleep(1); // Giữ cho massif snapshot
    free(a);
}
void bar() {
    char *b = malloc(300 * 1024); // 300KB
    memset(b, 'B', 300 * 1024);
    // intentionally NOT freeing b (memory leak)
    sleep(1);
}
void lots_of_buffers() {
    char *bufs[N_BUF];
    for (int i = 0; i < N_BUF; i++) {
        bufs[i] = malloc(BUF_SIZE); // 100KB mỗi buffer
        memset(bufs[i], 'C', BUF_SIZE);
    }
    sleep(1);
    for (int i = 0; i < N_BUF; i++) {
        free(bufs[i]);
    }
}
int main() {

    foo();             // 500KB alloc/free
    bar();             // 300KB alloc, không free
    lots_of_buffers(); // 10*100KB = 1MB alloc/free
    getchar(); // Để massif có thể chụp snapshot sau peak
    return 0;
}
