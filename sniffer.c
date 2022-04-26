#include<stdio.h>
#include<pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>

void packet_processor(u_char* args, const struct pcap_pkthdr *header, const u_char *packet) {
    // Referenced from: https://www.devdungeon.com/content/using-libpcap-c
    struct ether_header *eth_header;
    eth_header = (struct ether_header *) packet;
    if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
        printf("Not an IP packet. Skipping...\n\n");
        return;
    }
    const u_char *ip_header;
    const u_char *udp_header;
    const u_char *payload;

    int ethernet_header_length = 14; 
    int ip_header_length;
    int udp_header_length;
    int payload_length;

    ip_header = packet + ethernet_header_length;
    ip_header_length = ((*ip_header) & 0x0F);
    ip_header_length = ip_header_length * 4;
    u_char protocol = *(ip_header + 9);
    if (protocol != IPPROTO_UDP) {
        printf("Not a UDP packet. Skipping...\n");
        return;
    }

    udp_header = packet + ethernet_header_length + ip_header_length;
    // Length should be at starting at the 5th byte
    udp_header_length = ((*(udp_header + 10)) & 0xF0) >> 4;
    udp_header_length = udp_header_length * 4;
    printf("UDP header length in bytes: %d\n", udp_header_length);

    // Get payload offset
    int total_headers_size = ethernet_header_length+ip_header_length+udp_header_length;
    printf("Size of all headers combined: %d bytes\n", total_headers_size);
    payload_length = header->caplen - (ethernet_header_length + ip_header_length + udp_header_length);
    printf("Payload size: %d bytes\n", payload_length);
    payload = packet + total_headers_size;

    if (payload_length > 0) {
        const u_char *temp_pointer = payload;
        int byte_count = 0;
        while (byte_count++ < payload_length) {
            printf("%c", *temp_pointer);
            temp_pointer++;
        }
        printf("\n");
    }
}

void sniff_packet() {
    char* interface = "docker0";
    char err_buff[PCAP_ERRBUF_SIZE];
    pcap_t* handle;
    struct bpf_program filter;
    char* filter_port = "port 53";               // Capture DNS packets only
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