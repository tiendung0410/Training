
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
int *global_ptr = NULL; // For still reachable

void case_definitely_lost() {
    int *a = malloc(100); // Không free, mất hết pointer
    // Definitely lost
    (void)a;
}

void case_indirectly_lost() {
    int **b = malloc(sizeof(int*));    // Parent block
    *b = malloc(100);                  // Child block
    // Không free Parent block => Child block thành indirectly lost
    (void)b;
}

static void * thread_start(void *arg)
{
    //do nothing, just return
    return NULL; // do not detach
}
void case_possibly_lost() {
    pthread_t tid;
    pthread_create(&tid, NULL, &thread_start, (void*)100);
    // do not join
}

void case_still_reachable() {
    global_ptr = malloc(100); // Lưu vào global, không free

}

int main() {
    case_definitely_lost();
    case_indirectly_lost();
    case_possibly_lost();
    case_still_reachable();
    return 0;
}
