#include<stdio.h>
#include<pcap.h>

void packet_processor() {
    printf("Packet\n");
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