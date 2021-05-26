#ifndef SEARCH_H
#define SEARCH_H

#include "vector.h"

int search(vector_t vector, int (*compare)(const void *a, const void *b), const void *item);
int compareString(const void *ap, const void *bp);

#endif