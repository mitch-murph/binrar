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

int init_hashmap(hashmap_t *hashmap, size_t element_size, int (*compare)(const void *a, const void *b), int (*hash)(const void *a));
int free_hashmap(hashmap_t *hashmap);
int set_hashmap(hashmap_t hashmap, void *item);
void *get_hashmap(hashmap_t hashmap, void *item);

#endif