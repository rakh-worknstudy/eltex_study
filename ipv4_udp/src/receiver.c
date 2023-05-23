#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
//	----------------------------------------------------------------
	int socket_fd;
	printf("socket(): ");
	if ((socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
		printf("FAIL\n");
		exit(-1);
	}
	printf("SUCCESS\n");
	// int optval = 1;
	// if (setsockopt(socket_fd, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(optval)) < 0) {
	// 	perror("setsockopt");
	// 	return -1;
	// }
//	----------------------------------------------------------------
	struct sockaddr_in sv_address;
	printf("sockaddr_in: ");
	bzero(&sv_address, sizeof(sv_address));
	sv_address.sin_family = AF_INET;
	sv_address.sin_addr.s_addr = inet_addr("10.0.2.15");
	sv_address.sin_port = htons(8888);
	printf("SUCCESS\n");
	printf("bind(): ");
	if ((bind(socket_fd, (struct sockaddr *)&sv_address, sizeof(sv_address))) < 0) {
		printf("FAIL\n");
		exit(-1);
	}
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	while (1) {
		char buffer[1024];
		recv(socket_fd, buffer, 1024, 0);
		printf("cl_recv: %s\n", buffer + sizeof(struct iphdr) + sizeof(struct udphdr));
	}
//	----------------------------------------------------------------
	return 0;
}