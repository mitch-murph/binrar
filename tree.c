#include <stdio.h>
#include "tree.h"

void init_node(node_t *node, void* ptr)
{
    node->ptr = ptr;
    node->left = NULL;
    node->right = NULL;
}

int combine_tree(tree_t *tree, node_t *left, node_t *right)
{
    /* TODO: is != null needed? */
    if (left != NULL)
    {
        tree->left = left;
    }
    if (right != NULL)
    {
        tree->right = right;
    }
    return 0;
}