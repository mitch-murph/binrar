#ifndef SORT_H
#define SORT_H

#include "vector.h"

void bubble_sort(vector_t vector, int (*compare)(const void *a, const void *b));
void insertion_sort(vector_t vector, int (*compare)(const void *a, const void *b));

#endif