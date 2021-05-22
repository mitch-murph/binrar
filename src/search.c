#include "search.h"

int linear_search(vector_t vector, int (*compare)(const void *a))
{
    int i;
    for (i = 0; i < vector.size; i++)
    {
        if (compare(vector_get(vector, i)))
            return i;
    }
    return -1;
}