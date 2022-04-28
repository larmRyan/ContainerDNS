#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "host_updater.h"
#include "PopTracker.h"


#define MAX_BYTES 100
#define FAIL -1

#define IP 0
#define URL 1
#define CON 2
int THRESHOLD = 3;

char* entries_list[2];
extern char* host_files_list[255];


/** https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm */
char **parse_data(char *data) {

    static char *ret[3];
    char *delim = "*";

    int i = 0;

    char *token = strtok(data, delim);
    while(token != NULL) {
        ret[i] = token;
        token = strtok(NULL, delim);
        i += 1;
    }

    // We're missing data or wrong string sent over the pipe
    if(i != 2) {
        return NULL;
    }

    // Check if parsing is required
    char *space = strstr(ret[IP], " ");
    char *token2;
    if(space) {
        token2 = strtok(ret[IP], " ");
        token2 = strtok(NULL, " ");
        if(token2 != NULL) {
            strcpy(ret[IP], token2);
        }
    }
    return ret;
}

int main(int argc, char* argv[]) {
    if(argc > 2) {
        printf("Usage: sudo ./Middleware <threshold>\n");
    }
    else if (argc == 2) {
        THRESHOLD = atoi(argv[1]);
        printf("Setting threshold to: %d\n", THRESHOLD);
    }
    else{
        printf("Using default threshold: %d\n", THRESHOLD);
    }

    char buf[MAX_BYTES];
    char *fifo_path = "/tmp/cdns";
    char* host_path = "/home/ubuntu/522/ContainerDNS/test/";

    int pfd;
    tree_t *tree;

    mkfifo(fifo_path, 0666);

    // Open the named pipe for reading
    pfd = open(fifo_path, O_RDONLY);
    if(pfd < 0) {
        perror("Can't open the fifo");
        exit(FAIL);
    }

    get_directories(host_path);

    while(1) {
        if(read(pfd, buf, MAX_BYTES) > 0) {
            char **data = parse_data(buf);

            if(data != NULL) {
                in_addr_t ip, con;
                ip = inet_addr(data[IP]);
                if(data[CON] != NULL) {
                    con = inet_addr(data[CON]);
                }
                else {
                    con = 1234567;
                }
                char *url = data[URL];

                if(tree == NULL) {
                    tree = create_tree(ip, THRESHOLD);
                }

                // Add given node to the tree
                node_t *node = search_tree(tree, ip);
                if(node == NULL) {
                    insert_tree(tree, ip);
                }

                add_container(tree, ip, con);

                // Add the IP to the Container's Hosts file
                int no_containers = get_container_count(host_path);

                for(int i=0; i<no_containers; i++) {
                    FILE *read_fp  = fopen(host_files_list[i], "a+");
                    if(read_fp == NULL) {
                        printf("%s\n", strerror(errno));
                    }
                    else {
                        write_to_hosts(read_fp, data[IP], data[URL]);
                    }
                    fclose(read_fp);
                }

                // Check if the threshold has been met
                if(list_full(tree, ip)) {
                    
                    remove_tree(tree, node);
                    free_node(node);

                    for(int i = 0; i < no_containers; i++) {
                        struct in_addr in;
                        in.s_addr = ip;
                        char *ip_str = inet_ntoa(in);

                        // Open parent hosts file for overwriting
                        FILE* host_fp = fopen("/etc/hosts", "a+");
                        if(host_fp == NULL) {
                            printf("Could not open parent hosts file");
                        }
                        printf("Threshold reached. Writing to parent and removing from children\n");
                        write_to_hosts(host_fp, ip_str, data[URL]);
                        remove_entry(host_files_list[i], ip_str);
                        fclose(host_fp);
                    }   
                }
            }
        }
    }
}