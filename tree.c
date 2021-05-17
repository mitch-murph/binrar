#include <stdio.h>
#include "tree.h"

void init_node(const void *a)
{
    node_t *node = (node_t *)a;
    node->in_use = 0;
    node->value = 0;
    node->left = NULL;
    node->right = NULL;
}

void init_node_from_node(const void *a, const void *b)
{
    node_t *dest = (node_t *)a;
    node_t *src = (node_t *)b;
    dest->in_use = src->in_use;
    dest->key = src->key;
    dest->value = src->value;
    dest->left = src->left;
    dest->right = src->right;
}