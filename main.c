#include <stdio.h>
#include <malloc.h>

int main(void) {
    int *test_00, *test_01;
    printf("Allocating variable\n");
    test_00 = (int *)malloc(sizeof(int));
    printf("Writing to allocated variable\n");
    *test_00 = 0;
    printf("Writing to unallocated variable\n");
    *test_01 = 1;
    printf("Freeing allocated variable\n");
    free(test_00);
    printf("Freeing unallocated variable\n");
    free(test_01);
    return 0;
}