/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Hashmap
 * Functions for using a hashmap.
*****************************************************************************/
#include <stdio.h> /* printf */
#include "hashmap.h"
#include "vector.h"

/*****************************************************************************
 * Private function prototypes
 *  - No private functions in this files.
 *  - All public function prototypes in header.
*****************************************************************************/

/*****************************************************************************
 * This function initialises a hashmap
 * Input:
 *   hashmapp - Pointer to hashmap to initialise.
 *   elementSize - Size of each element in the hashmap.
 *   size - The number positions to initiialise into the hashmap.
 *   compare - Pointer to the function that compares to hashmap nodes.
 *   hash - Pointer to the function that computes the hash.
 *   exists - Pointer to the function that checks if node exists.
 *   init - Pointer to function that initialises each node.
 * Post:
 *   The pointer to the hashmap, hashmapp, will be initialises.
*****************************************************************************/
int initHashmap(hashmap_t *hashmapp, size_t elementSize, int size,
                int (*compare)(const void *a, const void *b),
                int (*hash)(const void *a),
                int (*exists)(const void *a),
                void (*init)(const void *a))
{
    /* Initialise the hashmap map. */
    initVectorCap(&hashmapp->map, size, elementSize);

    /* If a initialiser function is passed, run it over each node. */
    if (init != NULL)
    {
        int i;
        for (i = 0; i < hashmapp->map.capacity; i++)
        {
            init(vectorGet(hashmapp->map, i));
        }
    }

    /* Set the hashmap function pointers to the pointers passed in. */
    hashmapp->compare = compare;
    hashmapp->hash = hash;
    hashmapp->exists = exists;
    return 0;
}

/*****************************************************************************
 * This function frees a hashmap
 * Input:
 *   hashmapp - Pointer to hashmap to free.
 * Post:
 *   The pointer to the hashmap, hashmapp, will be freed.
*****************************************************************************/
int freeHashmap(hashmap_t *hashmapp)
{
    /* Free the map vector, no other dynamically allocated fields. */
    freeVector(hashmapp->map);
    return 0;
}

/*****************************************************************************
 * This function sets a item in the hashmap.
 * Input:
 *   hashmap - Hashmap where the item is to be set.
 *   item - The item to set inside the hashmap.
 * Post:
 *   hashmap map will now contain item in the hashed position.
 * Return:
 *   0 - Success
 *   1 - Failure
*****************************************************************************/
int hashmapSet(hashmap_t hashmap, void *item)
{
    /* Compute the hash value of the item. */
    int hash = hashmap.hash(item);

    /* Set the value in the map to the item. */
    if (vectorSet(hashmap.map, item, hash))
    {
        printf("Error: Hashmap cannot set");
        return 1;
    }
    return 0;
}

/*****************************************************************************
 * This function gets an item in the hashmap.
 * Input:
 *   hashmap - Hashmap where the item is to be set.
 *   item - The item to query inside the hashmap.
 * Return:
 *   Pointer to the item inside the hashmap.
 *   If it cannot be found NULL is returned.
*****************************************************************************/
void *hashmapGet(hashmap_t hashmap, void *item)
{
    /* Compute the hash of the item. */
    int hash = hashmap.hash(item);

    /* Get the item at the hash */
    void *maybeItem = vectorGet(hashmap.map, hash);

    /* Check if the item exists and it matches the item passed to query */
    if (hashmap.exists(maybeItem) && hashmap.compare(item, maybeItem))
    {
        /* If it does, return the item */
        return maybeItem;
    }
    /* Otherwise, return the NULL */
    return NULL;
}