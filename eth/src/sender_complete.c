#include <arpa/inet.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
//	----------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
//	----------------------------------------------------------------
#include "lib/frameopt.h"

unsigned short csum(char *header, int size);

int main(void) {
//	----------------------------------------------------------------
	int socket_fd;
	if ((socket_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
		perror("socket()");
		return -1;
	}
//	----------------------------------------------------------------	
	char buffer[1024];
	memset(buffer, 0, 1024);
//	----------------------------------------------------------------
    struct ethhdr *eth_header;
    eth_header = (struct ethhdr *)buffer;
	unsigned short ethhdr_length = sizeof(struct ethhdr);
//	----------------------------------------------------------------
	struct iphdr *ip_header;
	ip_header = (struct iphdr *)(buffer + ethhdr_length);
	unsigned short iphdr_length = sizeof(struct iphdr);
//	----------------------------------------------------------------
	struct udphdr *udp_header;
	udp_header = (struct udphdr *)(buffer + ethhdr_length + iphdr_length);
	unsigned short udphdr_length = sizeof(struct udphdr);
//	----------------------------------------------------------------
	struct frameopt frame_header;
	frame_header.eth_header = eth_header;
	frame_header.ip_header  = ip_header;
	frame_header.udp_header = udp_header;
	frameopt_get(&frame_header);
//	----------------------------------------------------------------
	char *data;
	data = buffer + ethhdr_length + iphdr_length + udphdr_length;
	sprintf(data, "%19s", "testestestestest...\0");
	unsigned short data_length = 20;
//	----------------------------------------------------------------
	struct ifreq if_idx;
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, frame_header.if_name, 6);
	if (ioctl(socket_fd, SIOCGIFINDEX, &if_idx) < 0) {
		perror("ioctl(SIOCGIFINDEX)");
		return -1;
	}
//	----------------------------------------------------------------
	ip_header->tot_len = htons(iphdr_length + udphdr_length + data_length);
#	ifdef __FAVOR_BSD
	udp_header->uh_len = htons(udphdr_length + data_length);
#	else
	udp_header->len = htons(udphdr_length + data_length);
#	endif  /* __FAVOR_BSD */
	srand(time(NULL));
	struct sockaddr_ll addr;
	addr.sll_family = AF_PACKET;
	addr.sll_protocol = htons(ETH_P_IP);
	addr.sll_ifindex = if_idx.ifr_ifindex;
	addr.sll_halen = ETH_ALEN;
	addr.sll_addr[0] = frame_header.eth_header->h_dest[0];
	addr.sll_addr[1] = frame_header.eth_header->h_dest[1];
	addr.sll_addr[2] = frame_header.eth_header->h_dest[2];
	addr.sll_addr[3] = frame_header.eth_header->h_dest[3];
	addr.sll_addr[4] = frame_header.eth_header->h_dest[4];
	addr.sll_addr[5] = frame_header.eth_header->h_dest[5];
	printf("\n");
	while (1) {
		ip_header->id = htons(rand());
		ip_header->check = 0;
		ip_header->check = csum(&buffer[ethhdr_length], iphdr_length);
		printf("sv_send: %s\n", buffer + ethhdr_length + iphdr_length + udphdr_length);
		sendto(socket_fd, buffer, ethhdr_length + iphdr_length + udphdr_length + data_length, 0, (struct sockaddr *)&addr, sizeof(addr));
		sleep(2);
	}
//	----------------------------------------------------------------
	return 0;
}

unsigned short csum(char *header, int size) {
	unsigned sum = 0;
	unsigned short *ptr = (unsigned short *)header;
	
	for (int iter = 0; iter < size / 2; ++iter)
		sum += *ptr++;
	while (sum > 0xFFFF) {
		unsigned over = sum >> 16;
		sum = (sum & 0xFFFF) + over;
	}
	sum = ~sum;
	return (unsigned short) (sum & 0xFFFF);
}