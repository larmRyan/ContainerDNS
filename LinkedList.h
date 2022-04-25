#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

struct linked_node {
    struct linked_node *next;
    struct linked_node *prev;
    in_addr_t ip;
};

struct linked_list {
    struct linked_node *head;
};

struct linked_list *create_linked_list(in_addr_t ip);
struct linked_node *create_linked_node(in_addr_t ip);
struct linked_node *search_linked_list(struct linked_list *list, in_addr_t ip);
void insert_linked_node(struct linked_list *list, in_addr_t ip);
// struct linked_node *delete_linked_node(struct linked_list *list, in_addr_t ip);
void delete_linked_node(struct linked_list *list, in_addr_t ip);
void free_list(struct linked_list *list);
int get_num_containers(struct linked_list *list);

