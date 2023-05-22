#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
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
	sv_address.sin_addr.s_addr = INADDR_ANY
	sv_address.sin_port = htons(1337);
	printf("SUCCESS\n");
	if ((bind(socket_fd, (struct sockaddr *)&sv_address, sizeof(sv_address))) < 0) {
		printf("FAIL\n");
		exit(-1);
	}
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	int length = sizeof(sv_address);
	while (1) {
		char buffer[256];
		recv(socket_fd, buffer, 256, 0);
		printf("cl_recv: %s\n", buffer);
	}
//	----------------------------------------------------------------
	return 0;
}