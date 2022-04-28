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
#include "host_updater.h"
#include "PopTracker.h"


#define MAX_BYTES 100
#define FAIL -1

#define IP 0
#define URL 1
#define CON 2
#define THRESHOLD 1

char* entries_list[2];
char* host_files_list[255];


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

    return ret;
}

int main() {

    char buf[MAX_BYTES];
    char *fifo_path = "/tmp/cdns";
    char* host_path = "/home/ubuntu/522/ContainerDNS/test/";

    int pfd;
    tree_t *tree;

    // Open the named pipe for reading
    pfd = open(fifo_path, O_RDONLY);
    if(pfd < 0) {
        perror("Can't open the fifo");
        exit(FAIL);
    }

    while(1) {
        if(read(pfd, buf, MAX_BYTES) > 0) {
            char **data = parse_data(buf);

            if(data != NULL) {

                in_addr_t ip = inet_addr(data[IP]);
                in_addr_t con = inet_addr(data[CON]);
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
                    FILE *read_fp  = fopen(host_files_list[i], "r+");
                    write_to_hosts(read_fp);
                    fclose(read_fp);
                }

                // Check if the threshold has been met
                if(list_full(tree, ip)) {
                    remove_tree(tree, node);
                    free_node(node);

                    for(int i = 0; i < no_containers; i++) {

                        // Open file for overwriting
                        // FILE *fp = fopen(host_files_list[i], "w+");

                        struct in_addr in;
                        in.s_addr = ip;
                        char *ip_str = inet_ntoa(in);
                        remove_entry(host_files_list[i], ip_str);
                    }   
                }
            }
        }
    }
}