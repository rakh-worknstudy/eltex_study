#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int loop(int cl2sv, int sv2cl);
unsigned short eltex_strlen(const char *buffer);

int main(void) {
	int cl2sv, sv2cl;
	cl2sv = open("cl2sv", O_WRONLY);
	sv2cl = open("sv2cl", O_RDONLY);
	loop(cl2sv, sv2cl);
	close(cl2sv);
	close(sv2cl);
	return 0;
}

int loop(int cl2sv, int sv2cl) {
	char buffer[256 + 4 + 1];
	printf("ready\nmsg max length: 256\n");
	while (1) {
		printf("send: ");
		if (!scanf("%260s", buffer)) {
			printf("scanf error\n");
			continue;
		}
		write(cl2sv, buffer, 261);
		read(sv2cl, buffer, 261);
		unsigned short length = eltex_strlen(buffer);
		if (length == 4 && buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't') {
			printf("shutdown\n");
			break;
		}
		if (length)
			printf("recv: %s\n", buffer);
		else
			printf("catch recv fail\n");
	}
	return 0;
}
unsigned short eltex_strlen(const char *buffer) {
	unsigned short length = 0;
	for (char *ptr = (char *)buffer; *ptr; ++ptr) {
		if (++length >= 261)
			break;
	}
	return length;
}
