#include "host_updater.h"

char* entries[2];
char* host_files[255];

/** https://stackoverflow.com/questions/3501338/c-read-file-line-by-line */
/** 
 * Opens the file to copy the content currently there
 * and will overwrite the file with the copied content
 * minus the one line that contains the ip address
 */
void remove_entry(char *file, char *ip) {
    char *lines[255];
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    FILE *fp = fopen(file, "r");
    
    if(fp == NULL) {
        fprintf(stderr, "File pointer is NULL");
        return;
    }

    // Copy all but the one line that we want to remove
    int i = 0;
    while((read = getline(&line, &len, fp)) != -1) {
        
        // Get the IP address from the current line
        char *token = strtok(line, "\t");

        // Remove only the line that contains the IP address
        if(strcmp(ip, token) != 0) {
            lines[i] = line;
            i++;
        }
    }

    if(fclose(fp) < 0) {
        fprintf(stderr, "Couldn't close the file after copying");
        exit(-1);
    }
    
    // Reopen the file to overwrite it
    fp = fopen(file, "w");
    for(int j = 0; j < i; j++) {
        fputs(lines[j], fp);
    }

    if(fclose(fp) < 0) {
        fprintf(stderr, "Couldn't close the file to rewrite");
        exit(-1);
    }

}

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
                sprintf(host_files[i], parent_dir);
                sprintf(host_files[i] + strlen(host_files[i]), dir_entry->d_name);
                sprintf(host_files[i] + strlen(host_files[i]), "/hosts");
                printf("%s\n", host_files[i]);
                i++;
            }
        }
        
        
    }
    closedir(src);
}

int get_container_count(char* parent_dir) {
    int containers=0;
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
                containers++;
            }
        }
    }
    closedir(src);
    return containers;
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