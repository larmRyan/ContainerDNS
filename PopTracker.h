#pragma once

#include "LinkedList.h"


/**
 * Structure of node in the BST
 */
typedef struct node {
    struct node *left;
    struct node *right;
    struct node *parent;
    in_addr_t ip; // key
    // struct linked_list *list; // list of containers that have requested the key
    in_addr_t *containers;
    // int count;
} node_t;

typedef struct tree {
    node_t *root;
} tree_t;

node_t *search_tree(tree_t *tree, in_addr_t ip);
void add_container(tree_t *tree, in_addr_t ip, in_addr_t con);
void insert_tree(tree_t *tree, in_addr_t ip);
void remove_tree(tree_t *tree, node_t *node);
void transplant(tree_t *tree, node_t *u, node_t *v);
tree_t *create_tree(in_addr_t ip, int thresh);
int list_full(tree_t *tree, in_addr_t ip);
node_t *create_tree_node(in_addr_t ip);
node_t *minimum(node_t *node);
void free_tree(node_t *node);
void free_node(node_t *node);
