#include <stdio.h>

struct Struct_padded {
	char a;
	int b;
};
struct Struct_packed {
	char a;
	int b;
}__attribute__((packed));

int fill_n_print(char *start, int size, char array[]);
int main(void) {
	char array[8] = {'a', 'b', 0, 0, 0, 0, 0, 0};
	struct Struct_padded A;
	struct Struct_packed B;
	printf("Array: a | b | 0 | 0 | 0 | 0 | 0 | 0\n\n"); 
	char *ptr;
	//	padded:
	printf("Padded test\nMem:   a | 0 | 0 | 0 | b | b | b | b\n");
	ptr = (char *)&A;
	fill_n_print(ptr, sizeof(A), array);
	printf("char = %c, int = %d (= %c)\n\n", A.a, A.b, (char)A.b);
	//	__attribute__((packed)):
	printf("Packed test\nMem:   a | b | b | b | b\n");
	ptr = (char *)&B;
	fill_n_print(ptr, sizeof(B), array);
	printf("char = %c, int = %d (= %c)\n", B.a, B.b, (char)B.b);
	return 0;
}

int fill_n_print(char *ptr, int size, char array[]) {
	printf("Res:  ");
	for (int iter = 0; iter < size; ++iter) {
		*ptr = array[iter];
		printf(" %c ", (*ptr) ? (*ptr) : '0');
		++ptr;
		if (iter < size - 1)
			printf("|");
	}
	printf("\n");
	return 0;
}
