#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
//	----------------------------------------------------------------
	if (argc != 2) {
		printf("Usage: ./client.exe PORT\n");
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
	int length = sizeof(sv_address);
	while (1) {
		char buffer[256];
		printf("cl_send: ");
		scanf("%255s", buffer);
		sendto(socket_fd, buffer, strlen(buffer), 0, (struct sockaddr *)&sv_address, length);
		int n = recvfrom(socket_fd, buffer, 256, 0, (struct sockaddr *)&sv_address, &length);
		printf("cl_recv: %s\n", buffer);
	}
//	----------------------------------------------------------------
	return 0;
}