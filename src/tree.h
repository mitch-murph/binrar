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
    int bitLength;
    node_t *left;
    node_t *right;
};

typedef node_t tree_t;

void initNode(const void *node);
void initNodeFromNode(const void *dest, const void *src);
int compareNodeValue(const void *a, const void *b);
int compareNodeKey(const void *a, const void *b);

#endif