#ifndef _FRAMEOPT_H
#define _FRAMEOPT_H

#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

struct frameopt {
	struct ethhdr *eth_header;
	struct iphdr  *ip_header;
	struct udphdr *udp_header;
	char if_name[32];
};

int frameopt_get(struct frameopt *this);

int mac_parse(const unsigned *input, unsigned char *this);
int ip_parse(const unsigned *input, unsigned *this);

int print_eth(struct ethhdr *this, const char *if_name);
int print_ip(struct iphdr *this);
int print_udp(struct udphdr *this);
int print_menu(struct frameopt *this);

int get_eth(struct ethhdr *this, char *if_name);
int get_ip(struct iphdr *this);
int get_udp(struct udphdr *this);

#endif /* _FRAMEOPT_H */