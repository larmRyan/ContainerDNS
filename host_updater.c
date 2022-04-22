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
    // char buff[buff_len];
    // while(fgets(buff, buff_len, read_path)) {
    //     printf("%s\n", buff);

    // }
    char* line = NULL;
    ssize_t len = 0;
    ssize_t nread;
    if ((nread = getline(&line, &len, read_path)) == -1) {
        // If file was empty
        char * dns_entry = get_dns_entry();
        fputs(dns_entry, read_path);
    }
    else{
        rewind(read_path);
        while ((nread = getline(&line, &len, read_path)) != -1) {
            char* dns_entry = get_dns_entry();
            printf("%s\n", line);
            // Write to file if entry did not already exist
            // TODO: Remove spaces in both entries
            if(strcmp(dns_entry, line) != 0) {
                fputs(dns_entry, read_path);
            }
        }
    }
    free(line);
}

int main() {
    char* host_path = "/home/ubuntu/522/ContainerDNS/test/hosts";
    FILE *read_fp  = fopen(host_path, "r+");
    write_to_hosts(read_fp);
    fclose(read_fp);

    return 0;
}