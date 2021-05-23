#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_CAPACITY 10

int initVector(vector_t *vector, size_t element_size)
{
    initVectorCap(vector, DEFAULT_CAPACITY, element_size);
    return 0;
}

int initVectorCap(vector_t *vector, int capacity, size_t element_size)
{
    vector->size = 0;
    vector->capacity = capacity;
    vector->element_size = element_size;
    vector->vector = malloc(capacity * element_size);
    return 0;
}

int copyVector(vector_t *dest, const vector_t src)
{
    dest->size = src.size;
    dest->capacity = src.capacity;
    dest->element_size = src.element_size;
    dest->vector = malloc(dest->capacity * dest->element_size);

    int i;
    for (i = 0; i < src.size; i++)
    {
        vectorSet(*dest, vectorGet(src, i), i);
    }

    return 0;
}

void freeVector(vector_t vector)
{
    free(vector.vector);
}

void *vectorGet(vector_t vector, int index)
{
    return vector.vector + index * vector.element_size;
}

void *vectorPushBack(vector_t *vector, void *value)
{
    if (vector->size >= vector->capacity - 1)
    {
        vector_resize(vector, vector->capacity * 2);
    }
    void *ptr = vector->vector + vector->size * vector->element_size;
    memcpy(ptr, value, vector->element_size);
    vector->size++;
    return ptr;
}

void *vectorPop(vector_t *vector)
{
    if (vector->size > 0)
    {
        vector->size--;
        return vectorGet(*vector, vector->size);
    }
    return NULL;
}

void vector_resize(vector_t *vector, int new_capacity)
{
    void *old_vector = vector->vector;
    vector->vector = malloc(new_capacity * vector->element_size);
    memcpy(vector->vector, old_vector, vector->size * vector->element_size);
    vector->capacity = new_capacity;
    free(old_vector);
}

int vectorRemove(vector_t *vector, int position)
{
    if (vector->size < position)
    {
        return 1;
    }
    int i;
    for (i = position; i < vector->size - 1; i++)
    {
        void *ptr = vectorGet(*vector, i + 1);
        void *new_ptr = vectorGet(*vector, i);
        memcpy(new_ptr, ptr, vector->element_size);
    }
    vector->size--;
    return 0;
}

int vectorSet(vector_t vector, void *value, int position)
{
    if (vector.capacity < position)
    {
        return 1;
    }
    void *ptr = vectorGet(vector, position);
    memcpy(ptr, value, vector.element_size);
    return 0;
}

void vectorSwap(vector_t vector, int a, int b)
{
    void *temp = malloc(vector.element_size);
    void *ap = vectorGet(vector, a);
    void *bp = vectorGet(vector, b);
    memcpy(temp, ap, vector.element_size);
    memcpy(ap, bp, vector.element_size);
    memcpy(bp, temp, vector.element_size);
    free(temp);
}

void vector_sort(vector_t vector, int (*compare)(const void *a, const void *b))
{
    int i, j;
    for (i = 0; i < vector.size; i++)
    {
        for (j = 0; j < vector.size; j++)
        {
            if (compare(vectorGet(vector, i), vectorGet(vector, j)) > 0)
            {
                vectorSwap(vector, i, j);
            }
        }
    }
}