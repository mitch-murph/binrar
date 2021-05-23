#include "search.h"

int search(vector_t vector, int (*compare)(const void *a, const void *b), const void *item)
{
    int i;
    for (i = 0; i < vector.size; i++)
    {
        if (compare(vectorGet(vector, i), item))
            return i;
    }
    return -1;
}