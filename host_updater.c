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
    char* dns_entry = "127.0.0.1    localhost123\n";
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

void create_data(char *data, char *ip, char *url) {
  char *delim = "*";
  strncat(data, ip, 255);
  strncat(data, delim, 1);
  strncat(data, url, 255);
}

// int main() {
//     // Packet sniffing initialization
//     char buff[1024];
//     int no_containers = 0;
//     sprintf(buff, "tshark -i ");
//     sprintf(buff + strlen(buff), "docker0");
//     sprintf(buff + strlen(buff), " -f \"udp src port 53\" -a ");
//     sprintf(buff + strlen(buff), "duration:5");

//     char *pipe_path = "/tmp/myfifo";
//     if (remove(pipe_path) == 0) printf("Pipe deleted successfully");

//     // Open new named pipe linking to the Middleware and PopTracker
//     int fifo = mkfifo(pipe_path, 0666);
//     if(fifo == -1) {
//       fprintf(stderr, "Couldn't create the pipe for publishing data to middleware\n");
//       exit(-1);
//     }
    
//     // File writing initialization
//     char* host_path = "/home/ubuntu/522/ContainerDNS/test/";
//     get_directories(host_path);
//     while(1) {
//         sniff_packet(buff);
//         printf("IP: %s \t URL: %s\n", net_info.ip, net_info.url);
//         no_containers = get_container_count(host_path);
//         for(int i=0; i<no_containers; i++) {
//             FILE *read_fp  = fopen(host_files[i], "a+");
//             write_to_hosts(read_fp);
//             fclose(read_fp);
//         }

//         int ffd = open(pipe_path, O_WRONLY);

//         if(ffd < 0) {
//           fprintf(stderr, "Counldn't open the pipe for writing");
//           exit(-1);
//         }

//         // Create the data for the middleware and send it through to 
//         // the named pipe
//         char data[255];
//         create_data(data, net_info.ip, net_info.url);
//         write(ffd, data, buff_len);
//         close(ffd);
//     }

//     return 0;
// }