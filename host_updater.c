 #define _GNU_SOURCE
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define buff_len 255

/**
 * @brief Parse DNS query to get hosts entry to add to hosts file
 * 
 * @return char* 
 */
char* get_dns_entry() {
    char* dns_entry = "127.0.0.1    localhost\n";
    return dns_entry;
}

void write_to_hosts(FILE* read_path) {
    char buff[buff_len];
    while(fgets(buff, buff_len, read_path)) {
        printf("%s\n", buff);

    }
}

int main() {
    char* host_path = "/home/ubuntu/522_project/ContainerDNS/test/hosts";
    FILE *read_fp  = fopen(host_path, "r+");
    write_to_hosts(read_fp);
    fclose(read_fp);

    return 0;
}