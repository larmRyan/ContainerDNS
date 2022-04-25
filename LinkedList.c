#pragma once

#include "LinkedList.h"

/**
 * Algorithms implemented from Introduction to Algorithms, Thomas Cormen, 3e.
 */ 

/**
 * Creates the linked list
 */
struct linked_list *create_linked_list(in_addr_t ip) {
    struct linked_node *head = (struct linked_node *) malloc(sizeof(struct linked_node));
    head->next = NULL;
    head->prev = NULL;
    head->ip = -1;

    struct linked_list *list = (struct linked_list *) malloc(sizeof(struct linked_list));
    list->head = head;
    return list;
}

/**
 * Creates a new node from given IP
 */
struct linked_node *create_linked_node(in_addr_t ip) {
    struct linked_node *node = (struct linked_node *) malloc(sizeof(struct linked_node));
    node->next = NULL;
    node->prev = NULL;
    node->ip = ip;
    return node;
}

/**
 * Searches the linked list for given IP address
 */
struct linked_node *search_linked_list(struct linked_list *list, in_addr_t ip) {
    struct linked_node *node = list->head;

    while(node != NULL && node->ip != ip) {
        node = node->next;
    }
    return node;
}

/**
 * Inserts given IP address into the list
 */
void insert_linked_node(struct linked_list *list, in_addr_t ip) {
    struct linked_node *node = create_linked_node(ip);

    node->next = list->head;
    if(list->head != NULL) {
        list->head->next->prev = node;
    }
    list->head = node;
}

/**
 * Removes given IP from the linked list
 */
void delete_linked_node(struct linked_list *list, in_addr_t ip) {
    struct linked_node *node = search_linked_list(list, ip);

    if(node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }

    if(node->next != NULL) {
        node->next->prev = node->prev;
    }

    // return node;
}

/**
 * Frees the entire linked list
 */
void free_list(struct linked_list *list) {
    struct linked_node *temp = list->head;
    struct linked_node *next = temp->next;
    while(temp != NULL) {
        next = temp->next;
        free(temp);
        temp = next;
    }

    free(list);
}

/**
 * Counts the number of containers in the list
 */ 
int get_num_containers(struct linked_list *list) {
    struct linked_node *temp = list->head;
    int size = 0;
    while(temp != NULL) {
        size += 1;
        temp = temp->next;
    }

    return size;
}
