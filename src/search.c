/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Search
 * The implemenation of the search function.
*****************************************************************************/
#include <string.h> /* strcmp */
#include "search.h"

/*****************************************************************************
 * This function implement the linear search.
 * Input:
 *   vector - The vector to be searched.
 *   compare - The function to compare the values by.
 *   item - The item to search the vector for.
 * Return:
 *   The index of the value in the vector.
 *   If not found -1.
*****************************************************************************/
int search(vector_t vector, int (*compare)(const void *a, const void *b),
           const void *item)
{
    /* check each element sequentially to find the required key.*/
    int i;
    for (i = 0; i < vector.size; i++)
    {
        /* Check if the required key. */
        if (compare(vectorGet(vector, i), item))
            return i;
    }
    /* If not found return -1 */
    return -1;
}
/**/
/*****************************************************************************
 * This function compares two strings for the search function.
 * Input:
 *   ap - A void pointer to a string A
 *       Must be this type to satisfy search()
 *   bp - A void pointer to a string B
 *       Must be this type to satisfy search()
 * Return:
 *   1 - If they are equal.
 *   0 - If they are not equal.
*****************************************************************************/
int compareString(const void *ap, const void *bp)
{
    /* Cast type both arguments to strings. */
    char *stringA = (char *)ap;
    char *stringB = (char *)bp;
    /* Compare their values. */
    return !strcmp(stringA, stringB);
}
