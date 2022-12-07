#include <stdio.h>

#define N_START 100000000000000000.0
#define N_STEP  10.0

int main(void) {
	FILE *res = fopen("result.txt", "w");
	double number = N_START;
	for (int i = 0; i < 2000; ++i, number += N_STEP)
		fprintf(res, "%.25lf\n", number);
	fprintf(res, "Start: %lf\nStep:  %lf\n", N_START, N_STEP);
	fclose(res);
	return 0;
}
