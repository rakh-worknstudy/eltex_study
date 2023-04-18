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
	mkfifo("cl2sv", 0777);
	mkfifo("sv2cl", 0777);
	cl2sv = open("cl2sv", O_RDONLY);
	sv2cl = open("sv2cl", O_WRONLY);
	loop(cl2sv, sv2cl);
	close(cl2sv);
	close(sv2cl);
	unlink("cl2sv");
	unlink("sv2cl");
	return 0;
}

int loop(int cl2sv, int sv2cl) {
	char buffer[256 + 4 + 1];
	printf("ready\n");
	while (1) {
		read(cl2sv, buffer, 261);
		unsigned short length = eltex_strlen(buffer);
		if (length >= 257) {
			printf("msg too long (recv)\n");
			continue;
		}
		if (length == 4 && buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't') {
			printf("shutdown\n");
			break;
		}
		if (length) {
			printf("recv: %s\n", buffer);
			buffer[length++] = 'b';
			buffer[length++] = 'a';
			buffer[length++] = 'c';
			buffer[length++] = 'k';
			buffer[length]   = 0;
			printf("send: %s\n", buffer);
			write(sv2cl, buffer, 261);
			continue;
		}
	}
	return 0;
}
unsigned short eltex_strlen(const char *buffer) {
	unsigned short length = 0;
	for (char *ptr = (char *)buffer; *ptr; ++ptr) {
		if (++length >= 257)
			break;
	}
	return length;
}
