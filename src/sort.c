#include "vector.h"
#include "sort.h"
/*function to implement the sort*/
void sort(vector_t vector, int (*compare)(const void *a, const void *b))
{
    /*checks to see if the element needs to be moved to the adjacent position*/
    int i, j;
    for (j = 0; j < vector.size - 1; j++)
    {
        for (i = 0; i < vector.size - j - 1; i++)
        {
            /*compares the elements to see if further sorting is required. If it does it will run through it again and if not it terminates*/
            if (compare(vectorGet(vector, i), vectorGet(vector, i + 1)) > 0)
            {
                vectorSwap(vector, i, i + 1);
            }
        }
    }
}
