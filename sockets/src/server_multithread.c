#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthreads.h>
#include <timer.h>

#include "lib/eltex_buffer.h"

int server_main(void);
int server_init(int *socket_fd, struct sockaddr_in *server_address);
int server_loop(int socket_fd);

void *connection_open(void);
void *connection_loop(int connection_fd);
void connection_close(connection_list_t *clients);

int main(void) {
	return server_main();
}

//	Server function
int server_main(void) {
	int connection_count = 0;
	int socket_fd, connection_fd;
	struct sockaddr_in server_address;
	server_init(&socket_fd, &server_address);

	return 0;
}
//	================================================================
//	                     SERVER INITIALIZATION
//	================================================================
int server_init(int *socket_fd, struct sockaddr_in *server_address) {
//	----------------------------------------------------------------
	printf("Initializing socket: ");
	if ((*socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("FAIL\n");
		exit(-1);
	}
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	printf("Configuring address: ");
	eltex_bzero(server_address, sizeof(struct sockaddr_in));
	server_address->sin_family = AF_INET;
	server_address->sin_addr.s_addr = htonl(INADDR_ANY);
	server_address->sin_port = htons(5000);
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	printf("Binding connection:  ");
	if (bind(*socket_fd, (struct sockaddr *)server_address, sizeof(struct sockaddr_in)) == -1) {
		printf("FAIL\n");
		close(*socket_fd);
		exit(-1);
	}
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	printf("Trying to listen:    ");
	if (listen(*socket_fd, 5) == -1) {
		printf("FAIL\n");
		close(*socket_fd);
		exit(-1);
	}
	printf("SUCCESS\n");
//	----------------------------------------------------------------
	return 0;
}

//	================================================================
//	                 NEW CONNECTION INITALIZATION
//	================================================================
int connection_open(void) {
	int connection_fd;
	struct sockaddr_in *client_address = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	int length = sizeof(struct sockaddr_in);
	while (1) {
		if ((connection_fd = accept(socket_fd, (struct sockaddr *)sockaddr_in, &length)) == -1) {
			printf("New client FAILED to connect\n");
			continue;
		}
		printf("New client SUCCESSFULLY connected\n");
		break;
	}
	return 1;
}

/* CLOSING OLD CONNECTION */
void connection_close(int connection_fd, struct sockaddr_in *client_address) {
	free(client_address);
	close(connection_fd);
}