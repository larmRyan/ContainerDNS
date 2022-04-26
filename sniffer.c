#include<stdio.h>
#include<pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include<string.h>

// Referenced (and modified) from: https://stackoverflow.com/a/49199234
/* 4 bytes IP address */
typedef struct ip_address{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header{
    u_char  ver_ihl;        // Version (4 bits) + Internet header length (4 bits)
    u_char  tos;            // Type of service
    u_short tlen;           // Total length
    u_short identification; // Identification
    u_short flags_fo;       // Flags (3 bits) + Fragment offset (13 bits)
    u_char  ttl;            // Time to live
    u_char  proto;          // Protocol
    u_short crc;            // Header checksum
    ip_address  saddr;      // Source address
    ip_address  daddr;      // Destination address
    u_int   op_pad;         // Option + Padding
}ip_header;

/* UDP header*/
typedef struct udp_header{
    u_short sport;          // Source port
    u_short dport;          // Destination port
    u_short len;            // Datagram length
    u_short crc;            // Checksum
}udp_header;

// DNS header
typedef struct dns_header{
    u_short id;             // ID
    u_char qr;
    u_int opcode;
}dns_header;



void packet_processor(u_char* args, const struct pcap_pkthdr *header, const u_char *pkt_data) {
    // Referenced from: https://www.devdungeon.com/content/using-libpcap-c
    ip_header *ih;
    udp_header *uh;
    u_int ip_len;
    u_short sport,dport,udp_len;
    dns_header *dh;

    /* retrieve the position of the ip header */
    ih = (ip_header *) (pkt_data + 14); //length of ethernet header

    /* retrieve the position of the udp header */
    ip_len = (ih->ver_ihl & 0xf) * 4;
    uh = (udp_header *) ((u_char*)ih + ip_len);

    /* convert from network byte order to host byte order */
    sport = ntohs( uh->sport );
    dport = ntohs( uh->dport );
    udp_len = uh->len;
    printf("LEN: %d\n", udp_len);
    printf("CRC: %d\n", uh->crc);
    

    // Get DNS header position
    // dh = (dns_header*)((u_char*)pkt_data + total_headers_size);
    // printf("ID %d\n", dh->id);
    // printf("QR:%d\n", ntohs(dh->qr));

}

void sniff_packet() {
    char* interface = "docker0";
    char err_buff[PCAP_ERRBUF_SIZE];
    pcap_t* handle;
    struct bpf_program filter;
    char* filter_port = "dst port 53";               // Capture DNS packets only
    bpf_u_int32 ip;

    // Open interface for listening
    handle = pcap_open_live(interface, BUFSIZ, 1, -1, err_buff);
    if(handle == NULL) {
        printf("Error capturing packets: %s\n", err_buff);
    }

    // Compile the filter
    if(pcap_compile(handle, &filter, filter_port, 0, ip) == -1) {
        printf("Error compiling filter %s\n", pcap_geterr(handle));
    }

    // Apply filter
    if(pcap_setfilter(handle, &filter) == -1) {
        printf("Error applying filter %s\n", pcap_geterr(handle));
    }

    // Listen for packets
    pcap_loop(handle, -1, packet_processor, NULL);
}

int main() {
    sniff_packet();
    return 0;
}