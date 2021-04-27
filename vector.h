#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

struct vector
{
    int size;
    int capacity;
    size_t element_size;
    void *vector;
};

typedef struct vector vector_t;

int init_vector(vector_t *vector, int capacity, size_t element_size);
void free_vector(vector_t vector);
void print_vector(vector_t vector, void (*print_func)(int index, void* value));
void vector_push_back(vector_t *vector, void *value);
void* vector_get(vector_t vector, int index);
void vector_resize(vector_t *vector, int new_capacity);

#endif