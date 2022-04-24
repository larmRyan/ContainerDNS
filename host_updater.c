#define _GNU_SOURCE
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>

#define buff_len 255
char* entries[2];
char* host_files[255];                      // Paths to hosts files of containers

void get_directories(char* parent_dir) {
    int i=0;
    struct dirent* dir_entry;
    DIR* src = opendir(parent_dir);
    while((dir_entry = readdir(src)) != NULL)
    {
        struct stat st;
        if(strcmp(dir_entry->d_name, ".") != 0 && strcmp(dir_entry->d_name, "..") != 0) {
            if (fstatat(dirfd(src), dir_entry->d_name, &st, 0) < 0)
            {
                perror(dir_entry->d_name);
                continue;
            }
            if(S_ISDIR(st.st_mode)) {
                host_files[i] = (char*)malloc(255 * sizeof(char));
                sprintf(host_files[i], dir_entry->d_name);
                sprintf(host_files[i] + strlen(host_files[i]), "/hosts");
                printf("%s\n", host_files[i]);
                i++;
            }
        }
        
        
    }
    closedir(src);
}

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
    char* host_path = "/home/ubuntu/522/ContainerDNS/test/";
    get_directories(host_path);
    while(1) {
        for(int i=0; i<2; i++) {
            printf("%s\n", host_files[i]);
            FILE *read_fp  = fopen(host_files[i], "r+");
            write_to_hosts(read_fp);
            fclose(read_fp);
        }
    }

    return 0;
}