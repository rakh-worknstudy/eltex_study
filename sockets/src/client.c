#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "lib/eltex_buffer.h"

int client_main(void);
int client_init(int *socket_fd, struct sockaddr_in *c_address);
int client_loop(int socket_fd);

int main(void) {
	return client_main();
}

//	Server function
int client_main(void) {
	int socket_fd, connection_fd;
	struct sockaddr_in server_address;
	client_init(&socket_fd, &server_address);
	client_loop(socket_fd);
	return 0;
}
//	Server initialization function
int client_init(int *socket_fd, struct sockaddr_in *server_address) {
	/* int socket_fd */
	printf("Initializing socket:  ");
	if ((*socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("FAIL\n");
		exit(-1);
	}
	printf("SUCCESS\n");
	/* sockaddr_in server_address */
	printf("Configuring address:  ");
	eltex_bzero(server_address, sizeof(*server_address));
	server_address->sin_family = AF_INET;
	server_address->sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address->sin_port = htons(5000);
	printf("SUCCESS\n");
	/* connect */
	printf("Connecting to server: ");
	if (connect(*socket_fd, (struct sockaddr *)server_address, sizeof(*server_address)) == -1) {
		printf("FAIL\n");
		close(*socket_fd);
		exit(-1);
	}
	printf("SUCCESS\n");
	return 0;
}
//	Server loop function
int client_loop(int socket_fd) {
	/* recv & send */
	char buffer[261];
	while (1) {
		printf("================================\n");
		printf("send: ");
		scanf("%256s", buffer);
		send(socket_fd, buffer, 261, 0);
		recv(socket_fd, buffer, 261, 0);
		unsigned short length = eltex_strlen(buffer, 256);
		if (length > 261) {
			printf("msg too big\n");
			continue;
		}
		if (eltex_strcmp(buffer, length)) {
			printf("recv: exit\n");
			printf("shutdown\n");
			break;
		}
		if (length) {
			printf("recv: %s\n", buffer);
		} else {
			printf("catch recv error\n");
		}
	}
	return 0;
}