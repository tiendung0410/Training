
#include <stdio.h>
#include <stdlib.h>

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

void case_possibly_lost() {
    int *c = malloc(100);
    int *p = c + 1;
    p = NULL;
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
