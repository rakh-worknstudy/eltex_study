#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
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
	
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	printf("Initializing address: ");
	struct sockaddr_in sv_address;
	memset(&sv_address, 0, sizeof(sv_address));
	sv_address.sin_family = AF_INET;
	sv_address.sin_addr.s_addr = inet_addr("224.6.6.6");
	sv_address.sin_port = htons(SV_PORT);
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	printf("Changing interface: ");
	struct in_addr mc_interface;
	mc_interface.s_addr = inet_addr("127.0.0.1");
	if (setsockopt(socket_fd, IPPROTO_IP, IP_MULTICAST_IF, &mc_interface, sizeof(mc_interface)) < 0) {
		printf("FAIL\n");
		exit(-1);
	}
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