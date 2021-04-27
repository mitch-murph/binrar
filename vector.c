#include "vector.h"

struct vector {
    int size;
    int capacity;
    void **vector;
};

typedef struct vector vector_t;

vector_t *new_vector(int capacity){
    vector_t *vector = (vector_t*)malloc(sizeof(vector_t*));
    vector->size = 0;
    vector->capacity = capacity;
    vector->vector = (vector_t*)malloc(sizeof(void*));
} 