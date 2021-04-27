#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

vector_t *new_vector(int capacity)
{
    vector_t *vector = (vector_t *)malloc(sizeof(vector_t *));
    vector->size = 0;
    vector->capacity = capacity;
    vector->vector = (void **)malloc(sizeof(void *));
    return vector;
}

void free_vector(vector_t *vector)
{
    free(vector->vector);
    free(vector);
}

void print_vector(vector_t *vector)
{
    int i;
    printf("vector\n");
    printf("size: %d\n", vector->size);
    printf("capacity: %d\n", vector->capacity);
    for (i = 0; i < vector->size; i++)
    {
        printf("[%d] %p\n", i, vector->vector[i]);
    }
}

void vector_insert(vector_t *vector, void* value)
{
    int *size = &vector->size;
    vector->vector[*size] = value;
    (*size)++;
}