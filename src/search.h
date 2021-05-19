#ifndef SEARCH_H
#define SEARCH_H

#include "vector.h"

void linear_search(vector_t vector, int (*compare)(const void *a, const void *b));
void binary_search(vector_t vector, int (*compare)(const void *a, const void *b));

#endif