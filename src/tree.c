#include <stdio.h> /* printf */
#include "tree.h"

/*****************************************************************************
 * This function initialises a node. Setting the default values.
 * Input:
 *   arg - Pointer to the student to be initialised.
 *         Must be this type to satisfy initHashmap()
 * Post:
 *   node arg will now be initialised with default values.
*****************************************************************************/
void initNode(const void *arg)
{
    /* Cast type the node to node_t. */
    node_t *node = (node_t *)arg;
    /* Set to the initial values. */
    node->in_use = 0;
    node->value = 0;
    node->bitLength = 0;
    node->left = NULL;
    node->right = NULL;
}

/*****************************************************************************
 * This function initialises a node from an existing node values.
 * Input:
 *   a - Pointer to the node to be initialised.
 *       Must be this type to satisfy the hashmap.
 *   b - Pointer to the node to be copied from.
 *       Must be this type to satisfy the hashmap.
 * Post:
 *   node arg will now be initialised with default values.
*****************************************************************************/
void initNodeFromNode(const void *a, const void *b)
{
    /* Cast type the node to node_t. */
    node_t *dest = (node_t *)a;
    node_t *src = (node_t *)b;
    /* Copy all values across. */
    dest->in_use = src->in_use;
    dest->key = src->key;
    dest->value = src->value;
    dest->bitLength = 0;
    dest->left = src->left;
    dest->right = src->right;
}

/*****************************************************************************
 * This function compares a two node values.
 * Input:
 *   a - A void pointer to a node A of type node_t.
 *       Must be this type to satisfy the hashmap.
 *   b - A void pointer to a node B of type node_t.
 *       Must be this type to satisfy the hashmap.
 * Return:
 *   0 - node A is larger
 *   1 - node B is larger
*****************************************************************************/
int compareNodeValue(const void *a, const void *b)
{
    /* Cast type the node to node_t. */
    node_t *nodeA = *(node_t **)a;
    node_t *nodeB = *(node_t **)b;

#ifdef DEBUG
    printf("Compare node result: %d", nodeA->value - nodeB->value);
#endif

    /* Subtract their values to determine which is larger. */
    return nodeA->value - nodeB->value < 0;
}

/*****************************************************************************
 * This function compares a node values with a char.
 * Input:
 *   a - A void pointer to a node of type node_t.
 *       Must be this type to satisfy the hashmap.
 *   b - A void pointer to a key of type char.
 *       Must be this type to satisfy the hashmap.
 * Return:
 *   1 - The key matches the node
 *   0 - Not equal
*****************************************************************************/
int compareNodeKey(const void *a, const void *b)
{
    /* Cast type the node to node_t. */
    node_t *node = *(node_t **)a;
    unsigned char key = *(unsigned char *)b;

#ifdef DEBUG
    printf("Compare node result: %d", node->key == key);
#endif

    /* compare their values to determine if equal */
    return node->key == key;
}

/*****************************************************************************
 * This function compares two nodes by their key
 * Input:
 *   a - A void pointer to node A of type node_t.
 *       Must be this type to satisfy huffman compression.
 *   a - A void pointer to node B of type node_t.
 *       Must be this type to satisfy huffman compression.
 * Return:
 *   1 - The key matches the node
 *   0 - Not equal
*****************************************************************************/
int compareNodeNode(const void *a, const void *b)
{
    /* Cast type the node to node_t. */
    node_t *nodeA = (node_t *)a;
    node_t *nodeB = (node_t *)b;
    /* compare their values to determine if equal */
    return nodeA->key == nodeB->key;
}

/*****************************************************************************
 * This function computes the hash of a node
 * Input:
 *   arg - A void pointer to a node of type node_t.
 *         Must be this type to satisfy huffman compression.
 * Return:
 *   1 - The key matches the node
 *   0 - Not equal
*****************************************************************************/
int hashNode(const void *arg)
{
    /* Cast type the node to node_t. */
    node_t *node = (node_t *)arg;
    /* Compute the hash. */
    return node->key % 0x101;
}

/*****************************************************************************
 * This function checks if a node exists in a position.
 * Input:
 *   arg - A void pointer to a node of type node_t.
 *         Must be this type to satisfy huffman compression.
 * Return:
 *   1 - In use
 *   0 - Not in use
*****************************************************************************/
int nodeExists(const void *arg)
{
    /* Cast type the node to node_t. */
    node_t *as = (node_t *)arg;
    /* Return if it is in use. */
    return as->in_use;
}
