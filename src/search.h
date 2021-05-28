/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Search
 * The implemenation of the search function.
*****************************************************************************/
#ifndef SEARCH_H
#define SEARCH_H

#include "vector.h"

/*****************************************************************************
 * Public function prototypes
*****************************************************************************/
int search(vector_t vector, int (*compare)(const void *a, const void *b), const void *item);
int compareString(const void *ap, const void *bp);

#endif