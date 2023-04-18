#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "lib/eltex_buffer.h"

int server_main(void);
int server_init(int *socket_fd, struct sockaddr_in *server_address);
int server_loop(int socket_fd);

int main(void) {
	return server_main();
}

//	Server function
int server_main(void) {
	int socket_fd, connection_fd;
	struct sockaddr_in server_address;
	server_init(&socket_fd, &server_address);
	server_loop(socket_fd);
	close(socket_fd);
	return 0;
}
//	Server initialization function
int server_init(int *socket_fd, struct sockaddr_in *server_address) {
	/* int socket_fd */
	printf("Initializing socket: ");
	if ((*socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("FAIL\n");
		exit(-1);
	}
	printf("SUCCESS\n");
	/* sockaddr_in server_address */
	printf("Configuring address: ");
	eltex_bzero(server_address, sizeof(*server_address));
	server_address->sin_family = AF_INET;
	server_address->sin_addr.s_addr = htonl(INADDR_ANY);
	server_address->sin_port = htons(5000);
	printf("SUCCESS\n");
	/* bind */
	printf("Binding connection:  ");
	if (bind(*socket_fd, (struct sockaddr *)server_address, sizeof(*server_address)) == -1) {
		printf("FAIL\n");
		close(*socket_fd);
		exit(-1);
	}
	printf("SUCCESS\n");
	/* listen */
	printf("Trying to listen:    ");
	if (listen(*socket_fd, 5) == -1) {
		printf("FAIL\n");
		close(*socket_fd);
		exit(-1);
	}
	printf("SUCCESS\n");
	return 0;
}
//	Server loop function
int server_loop(int socket_fd) {
	int connection_fd;
	struct sockaddr_in client_address;
	int length = sizeof(client_address);
	/* accept */
	printf("Waiting for client:  ");
	if ((connection_fd = accept(socket_fd, (struct sockaddr *)&client_address, &length)) == -1) {
		printf("FAIL\n");
		close(socket_fd);
		exit(-1);
	}
	printf("SUCCESS\n");
	/* recv & send */
	char buffer[261];
	while (1) {
		printf("================================\n");
		read(connection_fd, buffer, 261);
		unsigned short length = eltex_strlen(buffer, 256);
		if (length > 256) {
			printf("msg too big\n");
			continue;
		}
		if (eltex_strcmp(buffer, length)) {
			printf("recv: exit\n");
			printf("shutdown\n");
			close(connection_fd);
			break;
		}
		printf("recv: %s\n", buffer);
		buffer[length++] = 'b';
		buffer[length++] = 'a';
		buffer[length++] = 'c';
		buffer[length++] = 'k';
		buffer[length] = 0;
		write(connection_fd, buffer, 261);
		printf("send: %s\n", buffer);
	}
	return 0;
}