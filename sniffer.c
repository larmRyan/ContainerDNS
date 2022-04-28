#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define buff_len 255

// Struct to hold container DNS info
typedef struct network_info {
    char url[255];
    char ip[255];
    char container_ip[255];
} network_info;

network_info net_info;

/**
 * @brief Sniff for DNS responses
 * 
 * @param buff pipe
 */
void sniff_packet(char* buff) {
    FILE* pipe_fd = popen(buff, "r");
    if (!pipe_fd) {
        fprintf(stderr, "popen failed.\n");
    }

    char buffer[2048];
    char buffer2[2048];
    char *filterA;
    char *filterIP;
    while (NULL != fgets(buffer, sizeof(buffer), pipe_fd)) {
        // Only get type A responses
        filterA = strstr(buffer, "A");
        if(filterA) {
            strcpy(buffer2, buffer);
            // split on newline
            char* token = strtok(buffer, "\n");
            char* token2 = strtok(buffer2, "DNS");
            if(token2 != NULL) {
                token2 = strtok(token2, "→");
            }
            while(token2 != NULL) {
                strcpy(net_info.container_ip, token2);
                token2 = strtok(NULL, " ");
            }
            // Split first half
            token = strtok(token, "A");
            // Split second half and get URL
            if(token != NULL) {
                token = strtok(NULL, "A");
            }
            if(token != NULL) {
                strcpy(net_info.url, token);
                while (token != NULL)
                {
                    token = strtok(NULL, "A");
                    if(token != NULL) {
                        // Only get ipv4 responses
                        filterIP = strstr(token, ":");
                        if(!filterIP) {
                            strcpy(net_info.ip, token);
                        }
                        break;
                    }
                }
            }
        }
    }
    pclose(pipe_fd);
}

/**
 * @brief Create data to store in tree
 * 
 * @param data 
 * @param ip 
 * @param url 
 */
void create_data(char *data, char *ip, char *url) {
    char *delim = "*";
    // strncat(data, ip, 255);
    // strncat(data, delim, 1);
    // strncat(data, url, 255);
    sprintf(data, ip, "%s");
    sprintf(data + strlen(data), delim, "%s");
    sprintf(data + strlen(data), url, "%s");
}

int main() {
    char buff[1024];
    char *pipe_path = "/tmp/cdns";
    int fifo;
    int ffd;

    // Delete fifo if it exists
    if (remove(pipe_path) == 0) printf("FIFO deleted successfully\n");

    mkfifo(pipe_path, 0666);
    if(fifo == -1) {
      fprintf(stderr, "Couldn't create the pipe for publishing data to middleware\n");
      exit(-1);
    }

    sprintf(buff, "tshark -i ");
    sprintf(buff + strlen(buff), "docker0");
    sprintf(buff + strlen(buff), " -f \"udp src port 53\" -a ");
    sprintf(buff + strlen(buff), "duration:5");

    while(1) {
        sniff_packet(buff);
        printf("IP: %s \t URL: %s\n", net_info.ip, net_info.url);


        ffd = open(pipe_path, O_WRONLY);
        if(ffd < 0) {
          fprintf(stderr, "Could not open the pipe for writing\n");
          exit(-1);
        }

        // Create the data for the middleware and send it through to 
        // the named pipe
        char data[1024];
        create_data(data, net_info.ip, net_info.url);
        write(ffd, data, buff_len);
        close(ffd);
    }
}