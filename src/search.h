#ifndef SEARCH_H
#define SEARCH_H

#include "vector.h"

int linear_search(vector_t vector, int (*compare)(const void *a));
int binary_search(vector_t vector, int (*compare)(const void *a));

#endif