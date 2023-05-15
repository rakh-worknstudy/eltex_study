#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SV_PORT 25015

int main(int argc, char *argv[]) {
//	----------------------------------------------------------------
//	if (argc != 2) {
//		printf("Usage: ./client.exe FD PORT\n");
//		exit(-1);
//	}
//	int port = atoi(argv[1]);
//	----------------------------------------------------------------
	int socket_fd;
	printf("Initializing socket: ");
	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("FAIL\n");
		exit(-1);
	}
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	struct sockaddr_in sv_address;
	printf("Initializing address: ");
	int len = sizeof(struct sockaddr_in);
	memset(&sv_address, 0, sizeof(sv_address));
	sv_address.sin_family = AF_INET;
	sv_address.sin_addr.s_addr = htonl(INADDR_ANY);
	sv_address.sin_port = htons(SV_PORT);
	int reuse = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
		perror("setsockopt(SO_REUSEADDR) failed");
#	ifdef SO_REUSEPORT
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0) 
		perror("setsockopt(SO_REUSEPORT) failed");
#	endif
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	printf("Binding: ");
	if ((bind(socket_fd, (struct sockaddr *)&sv_address, len)) < 0) {
		printf("FAIL\n");
		exit(-1);
	}
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	while (1) {
		char buffer[256];
		read(socket_fd, buffer, 256);
		printf("cl_recv: %s\n", buffer);
	}
	return 0;
}