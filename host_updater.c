 #define _GNU_SOURCE
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define buff_len 255
char* entries[2];

char* remove_spaces(char* input_string) {
    int j=0;
    char* trimmed = (char*)malloc(strlen(input_string) * sizeof(char));
    for(int i=0; i<strlen(input_string); i++) {
        if(input_string[i] != ' ') {
            trimmed[j++] = input_string[i];
        }
    }
    return trimmed;
}

/**
 * @brief Parse DNS query to get hosts entry to add to hosts file
 * 
 * @return char* 
 */
void get_dns_entry() {
    char* dns_entry = "127.0.0.1    laptop\n";
    // Create a copy with removed spaces
    char* dns_entry_copy = (char*)malloc(strlen(dns_entry) * sizeof(char));
    strncpy(dns_entry_copy, dns_entry, strlen(dns_entry));
    entries[0] = dns_entry;
    // Remove spaces from entry
    char* trimmed = remove_spaces(dns_entry_copy);
    entries[1] = trimmed;
}

void write_to_hosts(FILE* read_path) {
    char* line = NULL;
    ssize_t len = 0;
    ssize_t nread;
    if ((nread = getline(&line, &len, read_path)) == -1) {
        // If file was empty
        get_dns_entry();
        fputs(entries[0], read_path);
    }
    else{
        rewind(read_path);
        while ((nread = getline(&line, &len, read_path)) != -1) {
            get_dns_entry();
            char* trimmed_line = remove_spaces(line);
            // Write to file if entry did not already exist
            if(strcmp(entries[1], trimmed_line) != 0) {
                // Add original entry (with spaces) to file
                fputs(entries[0], read_path);
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