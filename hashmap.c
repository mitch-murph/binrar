#include "hashmap.h"
#include "vector.h"

#define HASHMAP_INIT_SIZE 0x101

int init_hashmap(hashmap_t *hashmap, size_t element_size, int (*compare)(const void *a, const void *b), int (*hash)(const void *a), void (*init)(const void *a))
{
    init_vector(&hashmap->map, HASHMAP_INIT_SIZE, element_size);

    if (init != NULL)
    {
        int i;
        for (i = 0; i < hashmap->map.capacity; i++)
        {
            init(vector_get(hashmap->map, i));
        }
    }
    hashmap->compare = compare;
    hashmap->hash = hash;
    return 0;
}

int free_hashmap(hashmap_t *hashmap)
{
    free_vector(hashmap->map);
    return 0;
}

int hashmap_set(hashmap_t hashmap, void *item)
{
    int hash = hashmap.hash(item);
    vector_insert(&hashmap.map, item, hash);
    return 0;
}

void *hashmap_get(hashmap_t hashmap, void *item)
{
    int hash = hashmap.hash(item);
    return vector_get(hashmap.map, hash);
}