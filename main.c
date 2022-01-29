#include <stdio.h>
#include <stdlib.h>

#include "alloc.h"

int main(void) {
    init_alloc();
    print_chunks();

    const int *ptr = alloc(10 * sizeof(int));
    printf("\nptr  = %p\n\n", (void *)ptr);
    print_chunks();

    const int *ptr2 = alloc(1 * sizeof(int));
    printf("\nptr2 = %p\n\n", (void *)ptr2);
    print_chunks();

    del(ptr);
    print_chunks();

    del(ptr2);
    print_chunks();

    const void *ptr3 = alloc(100 * sizeof(int));
    printf("\nptr3 = %p\n\n", (void *)ptr3);
    print_chunks();

    const int *ptr4 = alloc(1 * sizeof(int));
    printf("\nptr4 = %p\n\n", (void *)ptr4);
    print_chunks();

    del(ptr4);
    print_chunks();

    const int *ptr5 = alloc(1 * sizeof(int));
    printf("\nptr5 = %p\n\n", (void *)ptr5);
    print_chunks();

    end_alloc();
    return EXIT_SUCCESS;
}
