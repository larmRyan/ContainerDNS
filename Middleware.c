#include "PopTracker.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "PopTracker.h"


#define MAX_BYTES 100

#define IP 0
#define URL 1
#define CON 2
#define THRESHOLD 1


/** https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm */
char **parse_data(char *data) {

    static char *ret[3];
    char *example = "127.0.0.1:google.com";
    char *delim = "*";

    int i = 0;

    char *token = strtok(data, delim);
    while(token != NULL) {
        ret[i] = token;
        token = strtok(NULL, delim);
        i += 1;
    }

    // We're missing data or wrong string sent over the pipe
    if(i != 1) {
        return NULL;
    }

    return ret;
}

int main() {

    char buf[MAX_BYTES];
    char *fifo_path = "/tmp/cdns";
    
    // Open the named pipe for reading
    int pfd = open(fifo_path, O_RDONLY);
    if(pfd < 0) {
        perror("Can't open the fifo");
        exit(-1);
    }

    tree_t *tree;

    while(1) {
        if(read(pfd, buf, MAX_BYTES) > 0) {
            char **data = parse_data(buf);

            if(data != NULL) {

                in_addr_t ip = inet_addr(data[IP]);
                char *url = data[URL];

                if(tree == NULL) {
                    tree = create_tree(ip, THRESHOLD);
                } 

                // Search for the node given the IP
                // and create the node if it doesn't
                // exist
                node_t *node = search_tree(tree, ip);
                if(node == NULL) {
                    insert_tree(tree, ip);
                }

                add_container(tree, ip);

                // Check if the threshold has been met
                // Remove the given node and update 
                // the file
                if(list_full(tree, ip)) {
                    remove_tree(tree, node);

                    // TODO: update the file
                    
                    free_node(node);
                }
            }
            
        }
    }
}