#include "eltex_buffer.h"

unsigned short eltex_strlen(const char *src, unsigned short max) {
	unsigned short length = 0;
	for (char *ptr = (char *)src; *ptr; ++ptr) {
		if (++length > max || !length)
			break;
	}
	return length;
}
void *eltex_bzero(void *dst, unsigned n) {
	char *ptr = (char *)dst;
	for (; n; --n)
		*ptr++ = 0;
	return dst;
}
int eltex_strcmp(const char *src, unsigned short length) {
	if (length == 4 && src[0] == 'e' && src[1] == 'x' && src[2] == 'i' && src[3] == 't')
		return 1;
	return 0;
}