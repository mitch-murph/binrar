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

int vector_insert(vector_t *vector, void *value, int position)
{
    if (vector->size < position)
    {
        return 1;
    }
    int i;
    for (i = vector->size - 1; i >= position; i--)
    {
        void *ptr = vector_get(*vector, i);
        void *new_ptr = vector_get(*vector, i + 1);
        memcpy(new_ptr, ptr, vector->element_size);
    }
    void *ptr = vector_get(*vector, position);
    memcpy(ptr, value, vector->element_size);
    vector->size++;
    return 0;
}

int vector_remove(vector_t *vector, int position)
{
    if (vector->size < position)
    {
        return 1;
    }
    int i;
    for (i = position; i < vector->size - 1; i++)
    {
        void *ptr = vector_get(*vector, i + 1);
        void *new_ptr = vector_get(*vector, i);
        memcpy(new_ptr, ptr, vector->element_size);
    }
    vector->size--;
    return 0;
}

int vector_set(vector_t vector, void *value, int position)
{
    if (vector.capacity < position)
    {
        return 1;
    }
    void *ptr = vector_get(vector, position);
    memcpy(ptr, value, vector.element_size);
    return 0;
}

void vector_swap(vector_t vector, int a, int b)
{
    void *temp = malloc(vector.element_size);
    void *ap = vector_get(vector, a);
    void *bp = vector_get(vector, b);
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
            if (compare(vector_get(vector, i), vector_get(vector, j)))
            {
                vector_swap(vector, i, j);
            }
        }
    }
}