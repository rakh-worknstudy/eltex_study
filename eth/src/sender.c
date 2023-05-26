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

unsigned short csum(char *header, int size);

int main(void) {
//	----------------------------------------------------------------
	int socket_fd;
	if ((socket_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
		perror("socket");
		return -1;
	}
//	----------------------------------------------------------------
	const char ethname[] = "enp0s8";
	struct ifreq if_idx;
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ethname, 6);
	if (ioctl(socket_fd, SIOCGIFINDEX, &if_idx) < 0) {
		perror("ioctl(SIOCGIFINDEX)");
		return -1;
	}
//	----------------------------------------------------------------	
	char buffer[1024];
	memset(buffer, 0, 1024);
//	----------------------------------------------------------------	
    struct ethhdr *eth_header;
    eth_header = (struct ethhdr *)buffer;
	unsigned short ethhdr_length = sizeof(struct ethhdr);
    eth_header->h_dest[0] = 0x08;
    eth_header->h_dest[1] = 0x00;
    eth_header->h_dest[2] = 0x27;
    eth_header->h_dest[3] = 0x91;
	eth_header->h_dest[4] = 0x07;
	eth_header->h_dest[5] = 0x28;
    eth_header->h_source[0] = 0x08;
    eth_header->h_source[1] = 0x00;
    eth_header->h_source[2] = 0x27;
    eth_header->h_source[3] = 0x08;
	eth_header->h_source[4] = 0x18;
	eth_header->h_source[5] = 0x60;
    eth_header->h_proto = htons(ETH_P_IP);
//	----------------------------------------------------------------	
	struct iphdr *ip_header;
	ip_header = (struct iphdr *)(buffer + ethhdr_length);
	unsigned short iphdr_length = sizeof(struct iphdr);
	ip_header->ihl = 5;
	ip_header->version = 4;
	ip_header->tos = 16;
	ip_header->ttl = 64;
	ip_header->protocol = 17;
	ip_header->saddr = inet_addr("10.1.2.4");
	ip_header->daddr = inet_addr("10.1.2.5");
//	----------------------------------------------------------------	
	struct udphdr *udp_header;
	udp_header = (struct udphdr *)(buffer + ethhdr_length + iphdr_length);
	unsigned short udphdr_length = sizeof(struct udphdr);
#	ifdef __FAVOR_BSD
	udp_header->uh_sport = htons(8888);
	udp_header->uh_dport = htons(8888);
#	else
	udp_header->source = htons(8888);
	udp_header->dest = htons(8888);
#	endif  /* __FAVOR_BSD */

	char *data;
	data = buffer + ethhdr_length + iphdr_length + udphdr_length;
	sprintf(data, "%19s", "testestestestest...\0");
	unsigned short data_length = 20;
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
	addr.sll_addr[0] = 0x08;
	addr.sll_addr[1] = 0x00;
	addr.sll_addr[2] = 0x27;
	addr.sll_addr[3] = 0x91;
	addr.sll_addr[4] = 0x07;
	addr.sll_addr[5] = 0x28;
	//	printf("%u\n%u\n%u\n", ethhdr_length, iphdr_length, udphdr_length);
	while (1) {
		//	printf("%p ", buffer + ethhdr_length);
		ip_header->id = htons(rand());
		//	ip_header->id = 123;
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