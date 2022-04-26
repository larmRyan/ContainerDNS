#include<stdio.h>
#include<string.h>

void sniff_packet() {
    char url[255];
    char ip[255];
    char* interface = "docker0";
    char buff[1024];
    sprintf(buff, "tshark -i ");
    sprintf(buff + strlen(buff), "docker0");
    sprintf(buff + strlen(buff), " -f \"udp src port 53\" -a ");
    sprintf(buff + strlen(buff), "duration:3");
    while(1) {
        FILE* pipe_fd = popen(buff, "r");

        if (!pipe_fd) {
            fprintf(stderr, "popen failed.\n");
        }

        char buffer[2048];
        while (NULL != fgets(buffer, sizeof(buffer), pipe_fd)) {
            // split on newline
            char* token = strtok(buffer, "\n");
            // Split first half
            token = strtok(token, "A");
            // Split second half and get URL
            token = strtok(NULL, "A");
            strcpy(url, token);
            printf("%s\n", url);
            printf("%s\n", url);
            while (token != NULL)
            {
                token = strtok(NULL, "A");
                strcpy(ip, token);
                printf("%s\n", ip);
                break;
                
            }
        }

        pclose(pipe_fd);
    }
}

int main() {
    sniff_packet();
    return 0;
}