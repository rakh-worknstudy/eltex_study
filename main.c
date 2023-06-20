#include <stdio.h>
#include <malloc.h>

void draw(int size);

int main(void) {
    printf("Input size (< 65535): ");
    int size;
    if (scanf("%d", &size) != 1 || size <= 0 || size > 65535)
        return -1;
    draw(size);
    return 0;
}

void draw(int size) {
   //  FIRST HALF
    for (unsigned iter = 0; iter < size;)
        printf("%u\t", ++iter);
    printf("\n");
    unsigned *line = (unsigned *)malloc(size * sizeof(unsigned));
    line[size - 1] = size;
    for (unsigned at = 0; at < size / 2; ++at) {
        line[at] = 4 * ((unsigned)size - (2 * at) - 1) + ((at) ? line[at - 1] : 0);
        for (unsigned iter = 0; iter < size; ++iter) {
            if (iter == at)
                ;
            else if (iter < at)
                --line[iter];
            else if (iter < size - at - 1)
                line[iter] = line[iter - 1] + 1;
            else
                ++line[iter];
            printf("%u\t", line[iter]);
        }
        printf("\n");
    }
    //  SECOND HALF
    unsigned width = (size % 2) ? 0 : 1;
    for (; width < size / 2; ++width) {
        for (unsigned iter = 0; iter < size; ++iter) {
            if (iter < size / 2 - width)
                --line[iter];
            else if (iter > size / 2 + width)
                ++line[iter];
            else
                line[iter] = line[iter - 1] - 1;
            printf("%u\t", line[iter]);
        }
        printf("\n");
    }
    free(line);
    return;
}