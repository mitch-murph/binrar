#ifndef TREE_H
#define TREE_H

#include "vector.h"

struct hashmap
{
    vector_t map;
    int (*compare)(const void *a, const void *b);
    int (*hash)(const void *a);
};

typedef struct hashmap hashmap_t;

int init_hashmap(hashmap_t *hashmap, size_t element_size, int (*compare)(const void *a, const void *b), int (*hash)(const void *a), void (*init)(const void *a));
int free_hashmap(hashmap_t *hashmap);
int hashmap_set(hashmap_t hashmap, void *item);
void *hashmap_get(hashmap_t hashmap, void *item);

#endif