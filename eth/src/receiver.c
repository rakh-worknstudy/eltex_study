#include <arpa/inet.h>
#include <net/if.h>
//  #include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
//	----------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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
    struct sockaddr_ll addr;
    memset(&addr, 0, sizeof(addr));
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_IP);
    addr.sll_ifindex = if_idx.ifr_ifindex;
    addr.sll_halen = ETH_ALEN;
    //  addr.sll_protocol = 0x0800;
    if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind()");
        return -1;
    }
//	----------------------------------------------------------------
    char buffer[1024];

    unsigned short ethhdr_length = sizeof(struct ethhdr);
    unsigned short iphdr_length  = sizeof(struct iphdr);
    unsigned short udphdr_length = sizeof(struct udphdr);

    struct iphdr *ip_header;
	ip_header = (struct iphdr *)(buffer + ethhdr_length);

    char *data;
	data = buffer + ethhdr_length + iphdr_length + udphdr_length;
//	----------------------------------------------------------------
    int size;
	while (1) {
		recv(socket_fd, buffer, 1024, 0);
        printf("cl_recv: %s\n", buffer + ethhdr_length + iphdr_length + udphdr_length);
	}
//	----------------------------------------------------------------
	return 0;
}