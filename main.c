#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pwrd_check(void);

int main(void) {
	int pwrd_status;
	pwrd_status = pwrd_check();
	if (pwrd_status == 0) {
		puts("Bad password!");
	} else {
		puts("Access granted!");
	}
	return 0;
}

int pwrd_check(void) {
	puts("Enter password: ");
	char pwrd[12];
	gets(pwrd);
	return 0 == strcmp(pwrd, "test");
}
