#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include "vector.h"

typedef struct node node_t;
typedef node_t tree_t;

struct node
{
    void *ptr;
    node_t *left;
    node_t *right;
};

void init_node(node_t *node, void* ptr);
int combine_tree(tree_t *tree, node_t *left, node_t *right);

#endif