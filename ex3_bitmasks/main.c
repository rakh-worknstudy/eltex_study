#include <stdio.h>

int main(void) {
	int number = 123456;
	printf("number: %d | %.2X %.2X %.2X %.2X\n", number, 	(number & 0xFF000000) >> 24,
								(number & 0x00FF0000) >> 16,
								(number & 0x0000FF00) >> 8,
								(number & 0x000000FF));
	printf("doing bitmask magic...\n");
	int mask = 0x08420000;
	number |= mask;
	printf("number: %d | %.2X %.2X %.2X %.2X\n", number,    (number & 0xFF000000) >> 24, 
                                               	 		(number & 0x00FF0000) >> 16,
                                                        	(number & 0x0000FF00) >> 8,
                                                        	(number & 0x000000FF));
	return 0;
}
