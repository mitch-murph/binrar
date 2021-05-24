#include "vector.h"
#include "sort.h"

void sort(vector_t vector, int (*compare)(const void *a, const void *b))
{
    int i, j;
    for (j = 0; j < vector.size - 1; j++)
    {
        for (i = 0; i < vector.size - j - 1; i++)
        {
            if (compare(vectorGet(vector, i), vectorGet(vector, i + 1)) > 0)
            {
                vectorSwap(vector, i, i + 1);
            }
        }
    }
}