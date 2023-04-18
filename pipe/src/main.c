#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int server(int *cl2sv, int *sv2cl);
int client(int *cl2sv, int *sv2cl);
unsigned short eltex_strlen(const char *buffer, const unsigned short max);

int main(void) {
	int cl2sv[2];
	int sv2cl[2];
	pipe(cl2sv);
	pipe(sv2cl);
	if (fork())
		server(cl2sv, sv2cl);
	else
		client(cl2sv, sv2cl);
	return 0;
}

int server(int *cl2sv, int *sv2cl) {
	while (1) {
		char buffer[261];
		read(cl2sv[0], buffer, 261);
		unsigned short length = eltex_strlen(buffer, 256);
		if (length == 4 && buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't') {
			printf("sv_recv: exit\n");
			write(sv2cl[1], "exit", 5);
			printf("sv_send: exit\n");
			break;
		}
		if (length > 256) {
			printf("msg too big\n");
			continue;
		}
		if (length) {
			printf("sv_recv: %s\n", buffer);
			buffer[length++] = 'b';
			buffer[length++] = 'a';
			buffer[length++] = 'c';
			buffer[length++] = 'k';
			buffer[length] = 0;
			write(sv2cl[1], buffer, 261);
			printf("sv_send: %s\n", buffer);
		}
	}
	wait(NULL);
	close(cl2sv[0]);
	close(sv2cl[1]);
	printf("sv_shutdown\n");
	printf("================================\n");
	return 0;
}
int client(int *cl2sv, int *sv2cl) {
	printf("================================\n");
	while (1) {
		char buffer[261];
		printf("cl_send: ");
		scanf("%256s", buffer);
		write(cl2sv[1], buffer, 261);
		read(sv2cl[0], buffer, 261);
		unsigned short length = eltex_strlen(buffer, 260);
		if (length == 4 && buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't') {
			printf("cl_recv: exit\n");
			break;
		}
		if (length) {
			printf("cl_recv: %s\n", buffer);
		} else {
			printf("catch recv error\n");
		}
		printf("================================\n");
	}
	close(cl2sv[1]);
	close(sv2cl[0]);
	printf("================================\n");
	printf("cl_shutdown\n");
	return 0;
}
unsigned short eltex_strlen(const char *buffer, const unsigned short max) {
	unsigned short length = 0;
	for (char *ptr = (char *)buffer; *ptr; ++ptr) {
		if (++length > max)
			break;
	}
	return length;
}
