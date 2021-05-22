#include "search.h"

int linearSearch(vector_t vector, int (*compare)(const void *a, const void *b), const void *item)
{
    int i;
    for (i = 0; i < vector.size; i++)
    {
        if (compare(vector_get(vector, i), item))
            return i;
    }
    return -1;
}