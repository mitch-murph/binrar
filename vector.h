#ifndef VECTOR_H
#define VECTOR_H

struct vector
{
    int size;
    int capacity;
    void **vector;
};

typedef struct vector vector_t;

vector_t *new_vector(int capacity);
void free_vector(vector_t *vector);
void print_vector(vector_t *vector);
void vector_insert(vector_t *vector, void* value);

#endif