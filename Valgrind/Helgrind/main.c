// test_helgrind.c - Demo race condition
#include <stdio.h>
#include <pthread.h>

int global_counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* thread_func(void* arg) {
    for (int i = 0; i < 100000; ++i) {
        // Lỗi race condition: 2 thread cùng ghi global_counter mà không bảo vệ!
        global_counter++;
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_func, NULL);
    pthread_create(&t2, NULL, thread_func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("global_counter = %d\n", global_counter);
    return 0;
}
