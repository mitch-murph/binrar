#ifndef SORT_H
#define SORT_H

#include "vector.h"

void sort(vector_t vector, int (*compare)(const void *a, const void *b));

#endif