#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SV_PORT 25015

int main(int argc, char *argv[]) {
//	----------------------------------------------------------------
// 	if (argc != 2) {
// 		printf("Usage: ./server.exe PORT\n");
// 		exit(-1);
// 	}
// 	unsigned port = atoi(argv[1]);
//	----------------------------------------------------------------
	int socket_fd;
	printf("Initializing socket: ");
	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("FAIL\n");
		exit(-1);
	}
	int broadcast = 1;
	int ret = setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	struct sockaddr_in sv_address;
	int len = sizeof(struct sockaddr_in);
	printf("Initializing address: ");
	memset(&sv_address, 0, sizeof(sv_address));
	sv_address.sin_family = AF_INET;
	//	sv_address.sin_addr.s_addr = inet_addr("10.0.2.255");
	sv_address.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	sv_address.sin_port = htons(SV_PORT);
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	char buffer[256];
	buffer[0] = 'h';
	buffer[1] = 'e';
	buffer[2] = 'l';
	buffer[3] = 'l';
	buffer[4] = 'o';
	buffer[5] = '\0';
	while (1) {
		sendto(socket_fd, buffer, 6, 0, (struct sockaddr *)&sv_address, sizeof(struct sockaddr_in));
		printf("sv_send: %s\n", buffer);
		sleep(3);
	}
	return 0;
}