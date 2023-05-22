#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <unistd.h>

unsigned short csum(char *header, int size);
int packet_print(char *buffer, int length);

int main(void) {
//	----------------------------------------------------------------
	int socket_fd;
	if ((socket_fd = socket(PF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
		perror("socket");
		return -1;
	}
	int optval = 1;
	if (setsockopt(socket_fd, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(optval)) < 0) {
		perror("setsockopt");
		return -1;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1337);
//	----------------------------------------------------------------	
	char buffer[1024];
	memset(buffer, 0, 1024);

	struct iphdr *ip_header;
	ip_header = (struct iphdr *)buffer;
	ip_header->ihl = 5;
	ip_header->version = 4;
	ip_header->tos = 16;
	ip_header->ttl = 4;
	ip_header->protocol = 17;
	ip_header->saddr = inet_addr("127.0.0.1");
	ip_header->daddr = inet_addr("10.0.2.15");
	unsigned short iphdr_length = sizeof(struct iphdr);

	struct udphdr *udp_header;
	udp_header = (struct udphdr *)(buffer + iphdr_length);
#	ifdef __FAVOR_BSD
	udp_header->uh_sport = htons(1337);
	udp_header->uh_dport = htons(8888);
#	else
	udp_header->source = htons(1337);
	udp_header->dest = htons(8888);
#	endif  /* __FAVOR_BSD */
	unsigned short udphdr_length = sizeof(struct udphdr);

	char *data;
	data = buffer + iphdr_length + udphdr_length;
	sprintf(data, "%s", "testestestestest...");
	unsigned short data_length = 20;
//	----------------------------------------------------------------
	ip_header->tot_len = htons(iphdr_length + udphdr_length + data_length);
#	ifdef __FAVOR_BSD
	udp_header->uh_len = htons(udphdr_length + data_length);
#	else
	udp_header->len = htons(udphdr_length + data_length);
#	endif  /* __FAVOR_BSD */
	srand(time(NULL));
	while (1) {
		ip_header->id = htons(rand());
		ip_header->check = htons(csum(buffer, iphdr_length));
		printf("sv_send: testestestestest...\n");
		sendto(socket_fd, buffer, iphdr_length + udphdr_length + data_length, 0, (struct sockaddr *)&addr, sizeof(addr));
		sleep(2);
	}
//	----------------------------------------------------------------
	return 0;
}

unsigned short csum(char *header, int size) {
	unsigned long sum = 0;
	unsigned short *ptr = (unsigned short *)header;
	
	for (int iter = 0; iter < size / 2; ++iter)
		sum += *ptr++;
	if (size % 2)
		sum += (header[size - 1] << 8) & 0x0000FF00;
	while (sum & 0xFFFF0000) {
		unsigned over = sum & 0xFFFF0000;
		sum -= over;
		over >>= 16;
		sum += over;
	}
	sum = ~sum;
	sum &= 0x0000FFFF;
	return (unsigned short) sum;
}
