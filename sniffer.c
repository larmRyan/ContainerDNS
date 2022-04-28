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

/**
 * Sniffs data via tshark and sends it along the fifo
 */

typedef struct network_info {
    char url[255];
    char ip[255];
    char container_ip[255];
} network_info;

network_info net_info;

void sniff_packet(char* buff) {
    FILE* pipe_fd = popen(buff, "r");
    if (!pipe_fd) {
        fprintf(stderr, "popen failed.\n");
    }

    char buffer[2048];
    char buffer2[2048];
    while (NULL != fgets(buffer, sizeof(buffer), pipe_fd)) {
        strcpy(buffer2, buffer);
        // split on newline
        char* token = strtok(buffer, "\n");
        char* token2 = strtok(buffer2, "DNS");
        token2 = strtok(token2, "→");
        token2 = strtok(NULL, " ");
        token2 = strtok(NULL, " ");
        strcpy(net_info.container_ip, token2);
        // Split first half
        token = strtok(token, "A");
        // Split second half and get URL
        token = strtok(NULL, "A");
        if(token != NULL) {
            strcpy(net_info.url, token);
            while (token != NULL)
            {
                token = strtok(NULL, "A");
                strcpy(net_info.ip, token);
                break;
            }
        }
    }
    pclose(pipe_fd);
}

void create_data(char *data, char *ip, char *url) {
    char *delim = "*";
    strncat(data, ip, 255);
    strncat(data, delim, 1);
    strncat(data, url, 255);
}

int main() {
    char buff[1024];
    char *pipe_path = "/tmp/myfifo";
    int fifo;

    sprintf(buff, "tshark -i ");
    sprintf(buff + strlen(buff), "docker0");
    sprintf(buff + strlen(buff), " -f \"udp src port 53\" -a ");
    sprintf(buff + strlen(buff), "duration:5");

    while(1) {
        sniff_packet(buff);
        printf("IP: %s \t URL: %s\n", net_info.ip, net_info.url);

        int ffd = open(pipe_path, O_WRONLY);
        if(ffd < 0) {
          fprintf(stderr, "Counldn't open the pipe for writing");
          exit(-1);
        }

        // Create the data for the middleware and send it through to 
        // the named pipe
        char data[255];
        create_data(data, net_info.ip, net_info.url);
        write(ffd, data, buff_len);
        close(ffd);
    }
}