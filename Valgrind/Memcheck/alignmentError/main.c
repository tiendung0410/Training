#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
int main() {
    void *ptr_memalign = NULL;
    void *ptr_posix_memalign = NULL;
    void *ptr_aligned_alloc = NULL;

    // alignment không hợp lệ (không phải power of 2)
    ptr_memalign = memalign(15, 64);   

    // alignment không chia hết cho 2, không chia hết cho sizeof(void*)
    posix_memalign(&ptr_posix_memalign, 63, 64); 
    // alignment = 0 (không hợp lệ)
    ptr_aligned_alloc = aligned_alloc(0, 64);    

    free(ptr_memalign);
    free(ptr_posix_memalign);
    free(ptr_aligned_alloc);

    return 0;
}
