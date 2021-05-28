#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_CAPACITY 10

/*****************************************************************************
 * This function initialises a vector. Mallocing the memory.
 * Input:
 *   vector - Pointer to the vector to be initialised.
 *   elementSize - The size of each element in the vector.
 * Post:
 *   vector will now be initialised with default values.
 * Return:
 *   On sucess - 0.
*****************************************************************************/
int initVector(vector_t *vector, size_t elementSize)
{
    /* Initialise a vector with the default capacity. */
    initVectorCap(vector, DEFAULT_CAPACITY, elementSize);
    return 0;
}

/*****************************************************************************
 * This function initialises a vector. Mallocing the memory of some capacity.
 * Input:
 *   vector - Pointer to the vector to be initialised.
 *   capacity - The capacity of the initial vector.
 *   elementSize - The size of each element in the vector.
 * Post:
 *   vector will now be initialised with the given capacity.
 * Return:
 *   On sucess - 0.
*****************************************************************************/
int initVectorCap(vector_t *vector, int capacity, size_t elementSize)
{
    /* Set to the initial values. */
    vector->size = 0;
    vector->capacity = capacity;
    vector->elementSize = elementSize;
    /* Initialise a vector with the given capacity. */
    vector->vector = malloc(capacity * elementSize);
    return 0;
}

/*****************************************************************************
 * This function copies a vector from another vector.
 * Input:
 *   dest - Pointer to the vector to be initialised.
 *   src - Vector to be copied from.
 * Post:
 *   dest vector will now be initialised using the values of src vector.
 * Return:
 *   On sucess - 0.
*****************************************************************************/
int copyVector(vector_t *dest, const vector_t src)
{
    /* Copy all values across. */
    dest->size = src.size;
    dest->capacity = src.capacity;
    dest->elementSize = src.elementSize;
    dest->vector = malloc(dest->capacity * dest->elementSize);

    /* Copy each element in the src vector. */
    int i;
    for (i = 0; i < src.size; i++)
    {
        vectorSet(*dest, vectorGet(src, i), i);
    }

    return 0;
}

/*****************************************************************************
 * This function frees a vector. 
 * Input:
 *   vector - The vector to be freed.
 * Post:
 *   vector will now be freed.
*****************************************************************************/
void freeVector(vector_t vector)
{
    /* Free the allocated memory of the vector. */
    free(vector.vector);
}

/*****************************************************************************
 * This function gets a value of an index in a vector. 
 * Input:
 *   vector - The vector to get the value from
 *   index - The position to get from the vector
 * Return:
 *   A pointer to the element of the given index.
*****************************************************************************/
void *vectorGet(vector_t vector, int index)
{
    /* Add the index multipled by element size to the start of the array.
       This gives the pointer of an element. */
    return vector.vector + index * vector.elementSize;
}

/*****************************************************************************
 * This function pushes a value onto a vector.
 * Input:
 *   vector - The vector to push the value onto
 *   value - The value to be inserted into the vector.
 * Return:
 *   A pointer to the element just pushed on
 * Post:
 *   Vector now contain the value.
*****************************************************************************/
void *vectorPushBack(vector_t *vector, void *value)
{
    /* Check if the value will if not, resize the vector. */
    if (vector->size >= vector->capacity - 1)
    {
        vectorResize(vector, vector->capacity * 2);
    }
    /* Copy the value by direct memory across. */
    void *ptr = vector->vector + vector->size * vector->elementSize;
    memcpy(ptr, value, vector->elementSize);
    /* Increase the size by 1 */
    vector->size++;
    /* Return the ptr to the new element. */
    return ptr;
}

/*****************************************************************************
 * This function pops the last value of a vector.
 * Input:
 *   vector - The vector to be popped.
 * Return:
 *   A pointer to the element that was just popped.
 *   If the vector was empty return NULL.
 * Post:
 *   Vector no longer contains the last value.
*****************************************************************************/
void *vectorPop(vector_t *vector)
{
    /* Ensure there is at least one value on the vector. */
    if (vector->size > 0)
    {
        /* Decrease the size by one and return the popped value. */
        vector->size--;
        return vectorGet(*vector, vector->size);
    }
    /* If not return NULL. */
    return NULL;
}

/*****************************************************************************
 * This function resizes a vector.
 * Input:
 *   vector - The vector to be resized.
 *   newCapacity - The new capacity of the vector.
 * Post:
 *   Vector will be resized to the new vector amount.
*****************************************************************************/
void vectorResize(vector_t *vector, int newCapacity)
{
    /* Store the current vector ptr. */
    void *oldVector = vector->vector;
    /* Allocate the resized memory. */
    vector->vector = malloc(newCapacity * vector->elementSize);
    /* Ensure the vector was resized. */
    if (vector->vector == NULL)
        return;
    /* Copy the old values to the new memory, set it to the vector ptr */
    memcpy(vector->vector, oldVector, vector->size * vector->elementSize);
    vector->capacity = newCapacity;
    /* Free the old vector. */
    free(oldVector);
}

/*****************************************************************************
 * This function removes an element from a vector.
 * Input:
 *   vector - The vector to have a value removed.
 *   position - The position to remove.
 * Post:
 *   Vector will no longer contain the value in that position.
 * Return:
 *   On sucess - 0.
*****************************************************************************/
int vectorRemove(vector_t *vector, int position)
{
    /* Ensure the position to be removed exists. */
    if (vector->size < position)
    {
        return 1;
    }
    /* Loop through all elements past the removed position backwards. */
    int i;
    for (i = position; i < vector->size - 1; i++)
    {
        /* Set the current value to the position ahead of it. */
        void *ptr = vectorGet(*vector, i + 1);
        void *new_ptr = vectorGet(*vector, i);
        memcpy(new_ptr, ptr, vector->elementSize);
    }
    /* Decrease the size by 1. */
    vector->size--;
    return 0;
}

/*****************************************************************************
 * This function sets an element in a vector.
 * Input:
 *   vector - The vector to have a value set.
 *   value - The value to set the position to.
 *   position - The position to be set.
 * Post:
 *   Vector will have the new value set in the position.
*****************************************************************************/
int vectorSet(vector_t vector, void *value, int position)
{
    /* Ensure the value exists in the array. */
    if (vector.capacity < position)
    {
        return 1;
    }
    /* Replace the value at the given position with the new value. */
    void *ptr = vectorGet(vector, position);
    memcpy(ptr, value, vector.elementSize);
    return 0;
}

/*****************************************************************************
 * This function swaps the values of two positions in a vector.
 * Input:
 *   vector - The vector to have the values swapped in.
 *   a - The position A to swap with.
 *   b - The position B to swap with.
 * Post:
 *   Vector will now have the values in position A and B swapped.
*****************************************************************************/
void vectorSwap(vector_t vector, int a, int b)
{
    /* Create temp space for the value. */
    void *temp = malloc(vector.elementSize);
    /* Get the two values. */
    void *ap = vectorGet(vector, a);
    void *bp = vectorGet(vector, b);
    /* Perform the swap. */
    memcpy(temp, ap, vector.elementSize);
    memcpy(ap, bp, vector.elementSize);
    memcpy(bp, temp, vector.elementSize);
    /* Free the temp space. */
    free(temp);
}