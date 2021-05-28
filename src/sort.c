/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Sort
 * The implemenation of the sort function.
*****************************************************************************/
#include "vector.h"
#include "sort.h"

/*****************************************************************************
 * Private function prototypes
 *  - No private functions in this files.
 *  - All public function prototypes in header.
*****************************************************************************/

/*****************************************************************************
 * This function implement the bubble sort.
 * Input:
 *   vector - The vector to be sorted.
 *   compare - The function to compare the values by.
 * Post:
 *   The index of the value in the vector.
 *   If not found -1.
*****************************************************************************/
void sort(vector_t vector, int (*compare)(const void *a, const void *b))
{
    /* Loop over all positions */
    int i, j;
    for (j = 0; j < vector.size - 1; j++)
    {
        /* Comparing them to each other position past it. 
           It will compares the elements to see if further 
            sorting is required. If it does it will run 
            through it again and if not it terminates */
        for (i = 0; i < vector.size - j - 1; i++)
        {
            /*checks to see if the element needs to be 
              moved to the adjacent position*/
            if (compare(vectorGet(vector, i), vectorGet(vector, i + 1)) > 0)
            {
                vectorSwap(vector, i, i + 1);
            }
        }
    }
}
