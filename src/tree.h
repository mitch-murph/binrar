/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Tree
 * Functions for a tree implemenation
*****************************************************************************/
#ifndef TREE_H
#define TREE_H

#include "vector.h"

/*****************************************************************************
 * Structs
*****************************************************************************/
/* Early typedef so that node_t can note_t can refer to itself.  */
typedef struct node node_t;

/* Struct to store information about a node.  */
struct node
{
    int in_use;
    unsigned char key;
    int value;
    int bitLength;
    node_t *left;
    node_t *right;
};

/* Second typedef so that node_t can be used a tree_t.  */
typedef node_t tree_t;

/*****************************************************************************
 * Public function prototypes
*****************************************************************************/
void initNode(const void *node);
void initNodeFromNode(const void *dest, const void *src);
int compareNodeValue(const void *a, const void *b);
int compareNodeKey(const void *a, const void *b);
int compareNodeNode(const void *a, const void *b);
int hashNode(const void *a);
int nodeExists(const void *a);

#endif