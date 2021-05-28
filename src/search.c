#include "search.h"
#include <string.h> /* strcmp */
/*function to implement the search*/
int search(vector_t vector, int (*compare)(const void *a, const void *b), const void *item)
{
    /*checks each element sequentially 
              to find the required key.*/
    int i;
    for (i = 0; i < vector.size; i++)
    {
        if (compare(vectorGet(vector, i), item))
            return i;
    }
    return -1;
}
/*compares two strings to see if the 
search algorith has been satisfied*/
int compareString(const void *ap, const void *bp)
{
    char *stringA = (char *)ap;
    char *stringB = (char *)bp;
    return !strcmp(stringA, stringB);
}
