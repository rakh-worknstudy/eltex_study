#include "frameopt.h"

#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>

#define SRC_MAC		"08:00:27:08:18:60"
#define SRC_IP		"10.1.2.4"
#define SRC_PORT	 8888

#define DST_MAC		"08:00:27:91:07:21"
#define DST_IP		"10.1.2.5"
#define DST_PORT	 8888

const unsigned src_mac[] = {0x08, 0x00, 0x27, 0x08, 0x18, 0x60};
const unsigned dst_mac[] = {0x08, 0x00, 0x27, 0x91, 0x07, 0x21};
const char ifdef[] = {'e', 'n', 'p', '0', 's', '8', '\0'};

#define CLEAR()		printf("\033[H\033[J")
#define GOTOXY(x,y)	printf("\033[%d;%dH", (y), (x))

int frameopt_get(struct frameopt *this) {
	/* Ethernet frame header */
	mac_parse(src_mac, this->eth_header->h_source);
	mac_parse(dst_mac, this->eth_header->h_dest);
	this->eth_header->h_proto = htons(ETH_P_IP);
	this->if_name[0] = ifdef[0];
	this->if_name[1] = ifdef[1];
	this->if_name[2] = ifdef[2];
	this->if_name[3] = ifdef[3];
	this->if_name[4] = ifdef[4];
	this->if_name[5] = ifdef[5];
	this->if_name[6] = ifdef[6];
	/* IP header */
	this->ip_header->ihl = 5;
	this->ip_header->version = 4;
	this->ip_header->tos = 16;
	this->ip_header->ttl = 64;
	this->ip_header->protocol = 17;
	this->ip_header->saddr = inet_addr(SRC_IP);
	this->ip_header->daddr = inet_addr(DST_IP);
	/* UDP header */
#	ifdef __FAVOR_BSD
	this->udp_header->uh_sport = htons(SRC_PORT);
	this->udp_header->uh_dport = htons(DST_PORT);
#	else
	this->udp_header->source = htons(SRC_PORT);
	this->udp_header->dest   = htons(DST_PORT);
#	endif  /* __FAVOR_BSD */
	print_menu(this);
}


int mac_parse(const unsigned *input, unsigned char *this) {
	for (int iter = 0; iter < 6; ++iter) {
		if (input[iter] > 255)
			return -1;
	}
	for (int iter = 0; iter < 6; ++iter)
		this[iter] = input[iter];
	return 0;
}
int ip_parse(const unsigned *input, unsigned *this) {
	for (int iter = 0; iter < 4; ++iter) {
		if (input[iter] > 255)
			return -1;
	}
	*this = 0;
	for (int iter = 0; iter < 4; ++iter) {
		*this *= 256;
		*this += input[iter];
	}
	*this = htonl(*this);
	return 0;
}

int print_eth(struct ethhdr *this, const char if_name[]) {
	GOTOXY(0, 1);
	printf("======================== L2 ========================\n");
	printf("Source MAC address:         %02X:%02X:%02X:%02X:%02X:%02X\n", this->h_source[0], this->h_source[1], this->h_source[2], this->h_source[3], this->h_source[4], this->h_source[5]);
	printf("Destination MAC address:    %02X:%02X:%02X:%02X:%02X:%02X\n", this->h_dest[0], this->h_dest[1], this->h_dest[2], this->h_dest[3], this->h_dest[4], this->h_dest[5]);
	printf("\33[2KInterface name:             %s\n", if_name);
}
int print_ip(struct iphdr *this) {
	GOTOXY(0, 5);
	printf("======================== L3 ========================\n");
	printf("\33[2KSource IP address:          %u.%u.%u.%u\n", ((unsigned char *)&(this->saddr))[0], ((unsigned char *)&(this->saddr))[1], ((unsigned char *)&(this->saddr))[2], ((unsigned char *)&(this->saddr))[3]);
	printf("\33[2KDestination IP address:     %u.%u.%u.%u\n", ((unsigned char *)&(this->daddr))[0], ((unsigned char *)&(this->daddr))[1], ((unsigned char *)&(this->daddr))[2], ((unsigned char *)&(this->daddr))[3]);
}
int print_udp(struct udphdr *this) {
	GOTOXY(0, 8);
	printf("======================== L4 ========================\n");
#	ifdef __FAVOR_BSD
	printf("\33[2KSource PORT:                %u\n", htons(this->uh_sport));
	printf("\33[2KDestination PORT:           %u\n", htons(this->uh_dport));
#	else
	printf("\33[2KSource PORT:                %u\n", htons(this->source));
	printf("\33[2KDestination PORT:           %u\n", htons(this->dest));
#	endif /* __FAVOR_BSD */
}
int print_menu(struct frameopt *this) {
	CLEAR();
	print_eth(this->eth_header, this->if_name);
	print_ip(this->ip_header);
	print_udp(this->udp_header);
	char ask = 'r';
	printf("====================================================\n1 - Start\n2 - Change L2\n3 - Change L3\n4 - Change L4\n0 - Exit\n");
	while (ask != '1') {
		scanf("%c", &ask);
		printf("\r\33[A\33[2K");
		switch (ask) {
		case '1':
			//	GOTOXY(1, 1);
			//	CLEAR();
			break;
		case '2':
			get_eth(this->eth_header, this->if_name);
			//	print_eth(this->eth_header);
			break;
		case '3':
			get_ip(this->ip_header);
			//	print_ip(this->ip_header);
			break;
		case '4':
			get_udp(this->udp_header);
			//	print_udp(this->udp_header);
			break;
		case '0':
			exit(0);
		default:
			break;
		}
		GOTOXY(0, 11);
		printf("====================================================\n1 - Start\n2 - Change L2\n3 - Change L3\n4 - Change L4\n0 - Exit\n");
	}
	return 0;
}

void clear_n(int n) {
	printf("\r");
	for (int i = 0; i < n; ++i)
		printf("\33[2K\33[A");
	printf("\33[2K");
}

int get_eth(struct ethhdr *this, char *if_name) {
	clear_n(5);
	printf("1 - Change Source MAC\n2 - Change Destination MAC\n3 - Change Ethernet interface\n0 - Return\n");
	char ask = 'r';
	while (ask != '0') {
		scanf("%c", &ask);
		unsigned vals[6];
		int res;
		switch (ask) {
		case '1':
			printf("\33[A");
			printf("Input src MAC (format \"SR:CA:DD:RE:SS\"): ");
			res = scanf("%x:%x:%x:%x:%x:%x", &vals[0], &vals[1], &vals[2], &vals[3], &vals[4], &vals[5]);
			printf("\r\33[A\33[2K");
			if (res == 6 && mac_parse(vals, this->h_source) == 0) {
				print_eth(this, if_name);
			}
			break;
		case '2':
			printf("\33[A");
			printf("Input dst MAC (format \"DS:TA:DD:RE:SS\"): ");
			res = scanf("%x:%x:%x:%x:%x:%x", &vals[0], &vals[1], &vals[2], &vals[3], &vals[4], &vals[5]);
			printf("\r\33[A\33[2K");
			if (res == 6 && mac_parse(vals, this->h_dest) == 0)
				print_eth(this, if_name);
			break;
		case '3':
			printf("\33[A");
			printf("Input interface name: ");
			scanf("%31s", if_name);
			print_eth(this, if_name);
			break;
		default:
			break;
		}
		GOTOXY(0, 16);
		printf("\33[2K");
	}
	printf("\r\33[A\33[2K");
	clear_n(4);
	return 0;
}
int get_ip(struct iphdr *this) {
	clear_n(5);
	printf("1 - Change Source IP\n2 - Change Destination IP\n0 - Return\n");
	char ask = 'r';
	while (ask != '0') {
		scanf("%c", &ask);
		unsigned vals[4];
		int res;
		switch (ask) {
		case '1':
			printf("\33[A");
			printf("Input src IP (format \"SRC.IPA.DDR.ESS\"): ");
			res = scanf("%u.%u.%u.%u", &vals[0], &vals[1], &vals[2], &vals[3]);
			printf("\r\33[A\33[2K");
			if (res == 4 && ip_parse(vals, &(this->saddr)) == 0)
				print_ip(this);
			break;
		case '2':
			printf("\33[A");
			printf("Input dst IP (format \"DST.IPA.DDR.ESS\"): ");
			res = scanf("%u.%u.%u.%u", &vals[0], &vals[1], &vals[2], &vals[3]);
			printf("\r\33[A\33[2K");
			if (res == 4 && ip_parse(vals, &(this->daddr)) == 0)
				print_ip(this);
			break;
		default:
			break;
		}
		GOTOXY(0, 15);
		printf("\33[2K");
	}
	printf("\r\33[A\33[2K");
	clear_n(3);
	return 0;
}
int get_udp(struct udphdr *this) {
	clear_n(5);
	printf("1 - Change Source Port\n2 - Change Destination Port\n0 - Return\n");
	char ask = 'r';
	while (ask != '0') {
		scanf("%c", &ask);
		unsigned val;
		int res;
		switch (ask) {
		case '1':
			printf("\33[A");
			printf("Input src Port: ");
			res = scanf("%u", &val);
			printf("\r\33[A\33[2K");
			if (res == 1) {
#				ifdef __FAVOR_BSD
				this->uh_sport = htons(val);
#				else
				this->source = htons(val);
#				endif  /* __FAVOR_BSD */
				print_udp(this);
			}
			break;
		case '2':
			printf("\33[A");
			printf("Input dst Port: ");
			res = scanf("%u", &val);
			printf("\r\33[A\33[2K");
			if (res == 1) {
#				ifdef __FAVOR_BSD
				this->uh_dport = htons(val);
#				else
				this->dest = htons(val);
#				endif  /* __FAVOR_BSD */
				print_udp(this);
			}
			break;
		default:
			break;
		}
		GOTOXY(0, 15);
		printf("\33[2K");
	}
	printf("\r\33[A\33[2K");
	clear_n(3);
	return 0;	
	return 0;
}