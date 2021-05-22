#ifndef SEARCH_H
#define SEARCH_H

#include "vector.h"

int linearSearch(vector_t vector, int (*compare)(const void *a, const void *b), const void *item);
int binary_search(vector_t vector, int (*compare)(const void *a, const void *b), const void *item);

#endif