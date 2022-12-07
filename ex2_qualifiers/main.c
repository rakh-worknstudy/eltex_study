#include <stdio.h>

int static_test(void) {
	static int d = 2;
	printf("static: %d\n", d);
	++d;
	return 0; 
}

int main(void) {
	extern int a;
	printf("extern: %d\n", a);
	printf("volatile: build with -S\n");
	int b = 2;
	{
		b = 3;
		b = 4;
		b = 5;
	}
	volatile int c = 2;
	{
		c = 3;
		c = 4;
		c = 5;
	}
	static_test();
	static_test();
	static_test();
	return 0;
}
