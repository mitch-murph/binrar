#include <stdio.h>
#include "tree.h"

void initNode(const void *a)
{
    node_t *node = (node_t *)a;
    node->in_use = 0;
    node->value = 0;
    node->bitLength = 0;
    node->left = NULL;
    node->right = NULL;
}

void initNodeFromNode(const void *a, const void *b)
{
    node_t *dest = (node_t *)a;
    node_t *src = (node_t *)b;
    dest->in_use = src->in_use;
    dest->key = src->key;
    dest->value = src->value;
    dest->bitLength = 0;
    dest->left = src->left;
    dest->right = src->right;
}

int compareNodeValue(const void *a, const void *b)
{
    node_t *nodeA = *(node_t **)a;
    node_t *nodeB = *(node_t **)b;
    return nodeA->value - nodeB->value < 0;
}

int compareNodeKey(const void *a, const void *b)
{
    node_t *node = *(node_t **)a;
    unsigned char key = *(unsigned char *)b;
    return node->key == key;
}

int compareNodeNode(const void *a, const void *b)
{
    node_t *as = (node_t *)a;
    node_t *bs = (node_t *)b;
    return as->key == bs->key;
}

int hashNode(const void *a)
{
    node_t *as = (node_t *)a;
    return as->key % 0x101;
}

int nodeExists(const void *a)
{
    node_t *as = (node_t *)a;
    return as->in_use;
}
