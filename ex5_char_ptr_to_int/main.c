#include <stdio.h>

int main(void) {
	int number = 12345678;
	printf("number: %d\nbytes: ", number);
	char *ptr = (char *)&number;
	for (int i = 0; i < sizeof(int); ++i, ++ptr)
		printf("%.2X ", (*ptr) & 0x000000FF);
	printf("\n");
	return 0;
}
