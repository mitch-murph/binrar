#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include "vector.h"

typedef struct tree tree_t;
typedef struct node node_t;

struct tree
{
    node_t *head;
    vector_t nodes;
    vector_t values;
};

int init_tree(tree_t *tree, size_t element_size);

void tree_get(vector_t vector, char pattern);

#endif