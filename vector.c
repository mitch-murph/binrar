#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int init_vector(vector_t *vector, int capacity, size_t element_size)
{
    vector->size = 0;
    vector->capacity = capacity;
    vector->element_size = element_size;
    vector->vector = malloc(capacity * element_size);
    return 0;
}

void free_vector(vector_t vector)
{
    free(vector.vector);
}

void print_vector(vector_t vector, void (*print_func)(int index, void *value))
{
    int i;
    printf("vector\n");
    printf("size: %d\n", vector.size);
    printf("capacity: %d\n", vector.capacity);
    for (i = 0; i < vector.size; i++)
    {
        print_func(i, vector.vector + i * vector.element_size);
    }
}

void *vector_get(vector_t vector, int index)
{
    return vector.vector + index * vector.element_size;
}

void vector_push_back(vector_t *vector, void *value)
{
    if (vector->size >= vector->capacity - 1)
    {
        vector_resize(vector, vector->capacity * 2);
    }
    void *ptr = vector->vector + vector->size * vector->element_size;
    memcpy(ptr, value, vector->element_size);
    vector->size++;
}

void vector_resize(vector_t *vector, int new_capacity)
{
    void *old_vector = vector->vector;
    vector->vector = malloc(new_capacity * vector->element_size);
    memcpy(vector->vector, old_vector, vector->size * vector->element_size);
    vector->capacity = new_capacity;
    free(old_vector);
}

void vector_insert(vector_t *vector, void *value, int position)
{
    if (vector->size < position)
    {
        vector->size = position;
        vector_push_back(vector, value);
    }
    else
    {
        void *ptr = vector->vector + position * vector->element_size;
        memcpy(ptr, value, vector->element_size);
    }
}