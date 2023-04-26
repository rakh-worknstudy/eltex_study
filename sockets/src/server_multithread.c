#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "lib/eltex_buffer.h"

typedef struct Connection_arg {
	int connection_fd;
	struct sockaddr_in client_address;
} connection_arg_t;
unsigned char cl_iter;
pthread_mutex_t cl_iter_lock;

int server_main(void);
int server_init(int *socket_fd, struct sockaddr_in *server_address);
int server_loop(int socket_fd);

int connection_wait(int socket_fd);
void *connection_open(void *arg);

int main(void) {
	return server_main();
}

//	Server function
int server_main(void) {
	int connection_count = 0;
	int socket_fd, connection_fd;
	struct sockaddr_in server_address;
	server_init(&socket_fd, &server_address);
	connection_wait(socket_fd);
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
int connection_wait(int socket_fd) {
	int connection_fd;
	int length = sizeof(connection_arg_t);
	connection_arg_t *arg;
	pthread_t sv_thread[5];
	cl_iter = 0;
	while (1) {
		if (cl_iter == 5) {
			printf("MAX CLIENTS\n");
			while (cl_iter == 5)
				continue;
		}
		printf("Allocating struct:   ");
		arg = (connection_arg_t *)malloc(sizeof(connection_arg_t));
		if (!arg) {
			printf("FAIL\n");
			continue;
		}
		printf("SUCCESS\n");
		if ((connection_fd = accept(socket_fd, (struct sockaddr *)&arg->client_address, &length)) == -1) {
			printf("New client FAILED to connect\n");
			free(arg);
			continue;
		}
		printf("New client SUCCESSFULLY connected\n");
		arg->connection_fd = connection_fd;
		printf("Creating new thread: ");
		if (pthread_create(&sv_thread[cl_iter], NULL, connection_open, (void *)arg)) {
			printf("FAIL\n");
			free(arg);
			continue;
		}
		printf("SUCCESS\n");
		++cl_iter;
	}
	return 0;
}

void *connection_open(void *arg) {
	/* recv & send */
	char buffer[261];
	int connection_fd = ((connection_arg_t *)arg)->connection_fd;
	while (1) {
		read(connection_fd, buffer, 261);
		unsigned short length = eltex_strlen(buffer, 256);
		if (length > 256) {
			printf("================================\n");
			printf("msg too big\n");
			continue;
		}
		if (eltex_strcmp(buffer, length)) {
			printf("================================\n");
			printf("recv: exit\n");
			printf("send: exit\n");
			close(connection_fd);
			break;
		}
		printf("================================\n");
		printf("recv: %s\n", buffer);
		buffer[length++] = 'b';
		buffer[length++] = 'a';
		buffer[length++] = 'c';
		buffer[length++] = 'k';
		buffer[length] = 0;
		write(connection_fd, buffer, 261);
		printf("send: %s\n", buffer);
	}
	while (pthread_mutex_trylock(&cl_iter_lock))
		continue;
	--cl_iter;
	pthread_mutex_unlock(&cl_iter_lock);
	close(connection_fd);
	free(arg);
	pthread_exit(NULL);
}