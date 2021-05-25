#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

struct vector
{
    int size;
    int capacity;
    size_t elementSize;
    void *vector;
};

typedef struct vector vector_t;

int initVector(vector_t *vector, size_t elementSize);
int initVectorCap(vector_t *vector, int capacity, size_t elementSize);
int copyVector(vector_t *dest, const vector_t src);
void freeVector(vector_t vector);
void vector_resize(vector_t *vector, int new_capacity);
void *vectorPushBack(vector_t *vector, void *value);
void *vectorPop(vector_t *vector);
void *vectorGet(vector_t vector, int index);
int vectorRemove(vector_t *vector, int position);
int vectorSet(vector_t vector, void *value, int position);
void vectorSwap(vector_t vector, int a, int b);

#endif