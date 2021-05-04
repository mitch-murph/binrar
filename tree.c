#include <stdio.h>
#include "tree.h"

typedef struct node node_t;

struct node
{
    void *value;
    node_t *left;
    node_t *right;
};

int init_tree(tree_t *tree, size_t element_size)
{
    tree->head = NULL;
    init_vector(&tree->nodes, 0, sizeof(node_t));
    init_vector(&tree->values, 0, element_size);
    return 0;
}