#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
//	----------------------------------------------------------------
	if (argc != 2) {
		printf("Usage: ./server.exe PORT\n");
		exit(-1);
	}
	int port = atoi(argv[1]);
//	----------------------------------------------------------------
	int socket_fd;
	printf("socket(...): ");
	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("FAIL\n");
		exit(-1);
	}
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	struct sockaddr_in sv_address;
	printf("sockaddr_in: ");
	bzero(&sv_address, sizeof(sv_address));
	sv_address.sin_family = AF_INET;
	sv_address.sin_addr.s_addr = INADDR_ANY;
	sv_address.sin_port = htons(port);
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	printf("bind(...): ");
	if (bind(socket_fd, (struct sockaddr *)&sv_address, sizeof(sv_address)) < 0) {
		printf("FAIL\n");
		exit(-1);
	}
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	struct sockaddr_in cl_address;
	int length = sizeof(cl_address);
	while (1) {
		char buffer[256];
		int n = recvfrom(socket_fd, buffer, 256, 0, (struct sockaddr *)&cl_address, &length);
		buffer[n] = 0;
		printf("sv_recv: %s\n", buffer);
		buffer[n++] = 'b';
		buffer[n++] = 'a';
		buffer[n++] = 'c';
		buffer[n++] = 'k';
		buffer[n++] =  0;
		sendto(socket_fd, buffer, n, 0, (struct sockaddr *)&cl_address, length);
		printf("sv_send: %s\n", buffer);
	}
//	----------------------------------------------------------------
	return 0;
}