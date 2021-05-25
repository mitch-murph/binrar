#ifndef HASHMAP_H
#define HASHMAP_H

#include "vector.h"

/*****************************************************************************
 * Structs
*****************************************************************************/
/* Struct to hashmap information */
struct hashmap
{
    vector_t map; /* The actual hashmap */
    int (*compare)(const void *a,
                   const void *b); /* Pointer to compare function */
    int (*hash)(const void *a);    /* Pointer to hash function */
    int (*exists)(const void *a);  /* Pointer to esist function*/
};

typedef struct hashmap hashmap_t;

/*****************************************************************************
 * Public function prototypes
*****************************************************************************/
int initHashmap(hashmap_t *hashmap, size_t elementSize, int size,
                int (*compare)(const void *a, const void *b),
                int (*hash)(const void *a),
                int (*exists)(const void *a),
                void (*init)(const void *a));
int freeHashmap(hashmap_t *hashmap);
int hashmapSet(hashmap_t hashmap, void *item);
void *hashmapGet(hashmap_t hashmap, void *item);

#endif