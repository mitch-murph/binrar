#include "vector.h"
#include "sort.h"

void bubble_sort(vector_t vector, int (*compare)(const void *a, const void *b))
{
    int i, j;
    for (j = 0; j < vector.size - 1; j++)
    {
        for (i = 0; i < vector.size - j - 1; i++)
        {
            if (compare(vector_get(vector, i), vector_get(vector, i + 1)) > 0)
            {
                vector_swap(vector, i, i + 1);
            }
        }
    }
}

void insertion_sort(vector_t vector, int (*compare)(const void *a, const void *b))
{
    void *item;
    int i, j, index;
    for (i = 1; i < vector.size; i++)
    {
        index = i;
        vector_push_back(&vector, vector_get(vector, index));
        /* item = vector_get(vector, i); */
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && compare(vector_get(vector, i), item))
        {
            j = j - 1;
        }

        /* vector_insert(&vector, vector_get(vector, index), j); */
    }
}