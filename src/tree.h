#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include "vector.h"

typedef struct node node_t;

struct node
{
    int in_use;
    unsigned char key;
    int value;
    int bit_length;
    node_t *left;
    node_t *right;
};

typedef node_t tree_t;

void init_node(const void *node);
void init_node_from_node(const void *dest, const void *src);

#endif