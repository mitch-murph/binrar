/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Sort
 * The implemenation of the sort function.
*****************************************************************************/
#ifndef SORT_H
#define SORT_H

#include "vector.h"

/*****************************************************************************
 * Public function prototypes
*****************************************************************************/
void sort(vector_t vector, int (*compare)(const void *a, const void *b));

#endif