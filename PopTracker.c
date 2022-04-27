#pragma once

#include "PopTracker.h"

int threshold = 1;

/**
 * Algorithms implemented from Introduction to Algorithms, Thomas Cormen, 3e.
 */ 

/**
 * Creates a new node for the BST
 */
node_t *create_tree_node(in_addr_t ip) {
    node_t *node = (node_t *) malloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->ip = ip;
    //node->containers = (in_addr_t *) malloc(sizeof(in_addr_t) * threshold);
    node->count = 0;
    return node;
}

/**
 * Instantiates the BST
 */ 
tree_t *create_tree(in_addr_t ip, int thresh) {

    threshold = thresh;
    tree_t *tree = (tree_t *) malloc(sizeof(tree_t));
    node_t *root = create_tree_node(ip);
    tree->root = root;
    return tree;
}

/**
 * Searches tree for given IP address
 */
node_t *search_tree(tree_t *tree, in_addr_t ip) {

    node_t *node = tree->root;
    while(node != NULL && node->ip != ip) {
        if(node->ip < ip) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return node;
}

/**
 * Checks if the list of containers is full
 */
int list_full(tree_t *tree, in_addr_t ip) {
    node_t *node = search_tree(tree, ip);

    // for(int i = 0; i < threshold * sizeof(in_addr_t); i += sizeof(in_addr_t)) {
    //     if(node->containers[i] == 0) {
    //         return 0;
    //     }
    // }
    if(node != NULL) {
        if(node->count < threshold) return 0;
    }
    return 1;
}

/**
 * Adds a new container to the list
 */
void add_container(tree_t *tree, in_addr_t ip) {
    node_t *node = search_tree(tree, ip);

    // for(int i = 0; i < threshold * sizeof(in_addr_t); i += sizeof(in_addr_t)) { 
    //     if(node->containers[i] == con) { // already in the list
    //         return;
    //     } else if(node->containers[i] == 0) {
    //         node->containers[i] = con;
    //     }
    // }

    if(node != NULL) {
        node->count++;
    }
}

/**
 * Inserts ip into the tree
 */ 
void insert_tree(tree_t *tree, in_addr_t ip) {
    node_t *y = NULL;
    node_t *x = tree->root;
    node_t *z = create_tree_node(ip);
    while(x != NULL) {
        y = x;
        if(z->ip < x->ip) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;

    if(y == NULL) { // Tree empty
        tree->root = z;
    } else if(z->ip < y->ip) {
        y->left = z;
    } else {
        y->right = z;
    }
}

/**
 * Transplants subtree when deleting nodes
 */
void transplant(tree_t *tree, node_t *u, node_t *v) {
    if(u->parent == NULL) {
        tree->root = v;
    } else if(u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    if(v != NULL) {
        v->parent = u->parent;
    }
}

/**
 * Removes give node from tree
 */ 
void remove_tree(tree_t *tree, node_t* node) {
    node_t *y = NULL;
    if(node->left == NULL) {
        transplant(tree, node, node->right);
    } else if(node->right == NULL) {
        transplant(tree, node, node->left);
    } else {
        y = minimum(node->right);

        if(y->parent != node) {
            transplant(tree, y, y->right);
            y->right = node->right;
            y->right->parent = y;
        }

        transplant(tree, node, y);
        y->left = node->left;
        y->left->parent = y;
    }
}

node_t *minimum(node_t *node) {
    while(node->left != NULL) {
        node = node->left;
    }
    return node;
}

void free_node(node_t *node) {
    if(node != NULL) {
        // free(node->containers);
        free(node);
    }
}

void free_tree(node_t *node) {
    if(node != NULL) {
        free_tree(node->left);
        free_tree(node->right);
        // free(node->containers);
        free(node);
    }
}
