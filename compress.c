#include "compress.h"
#include "hashmap.h"
#include "vector.h"
#include "tree.h"
#include <stdio.h>

/* PRIVATE */
typedef struct kvp kvp_t;

struct kvp
{
    int in_use;
    unsigned char key;
    int value;
};

void init_kvp(const void *a)
{
    kvp_t *node = (kvp_t *)a;
    node->in_use = 0;
    node->value = 0;
}

void count_bytes(char *filename, hashmap_t *map)
{
    int buffer;
    FILE *ptr;
    ptr = fopen(filename, "rb");

    while ((buffer = fgetc(ptr)) != EOF)
    {
        kvp_t node;
        node.key = buffer;

        kvp_t *node_maybe = (kvp_t *)hashmap_get(*map, (void *)&node);
        if (node_maybe != NULL)
        {
            node_maybe->value++;
            hashmap_set(*map, (void *)node_maybe);
        }
        else
        {
            init_kvp(&node);
            node.in_use = 1;
            node.value++;

            hashmap_set(*map, (void *)&node);
        }
    }

    fclose(ptr);
}

void func2(int pos, void *value)
{
    kvp_t *c = (kvp_t *)(value);
    printf("%p [%d]\t%02x\t%d\n", value, pos, c->key, c->value);
}

int comp(const void *a, const void *b)
{
    kvp_t *as = (kvp_t *)a;
    kvp_t *bs = (kvp_t *)b;
    return as->key == bs->key;
}

int comp2(const void *a, const void *b)
{
    kvp_t *as = (kvp_t *)a;
    kvp_t *bs = (kvp_t *)b;
    return bs->value - as->value < 0;
}

int hash(const void *a)
{
    kvp_t *as = (kvp_t *)a;
    return as->key % 0x101;
}

int exists(const void *a)
{
    kvp_t *as = (kvp_t *)a;
    return as->in_use;
}

void hashmap_convert_to_vector(hashmap_t map, vector_t *vector)
{
    int i;
    for (i = 0; i < map.map.capacity; i++)
    {
        void *item = vector_get(map.map, i);
        if (map.exists(item))
        {
            vector_push_back(vector, (void*)item);
        }
    }
}

/* PUBLIC */
void compress()
{
    hashmap_t map;
    init_hashmap(&map, sizeof(kvp_t), 0x101, comp, hash, exists, init_kvp);
    count_bytes("data-files/random-file.bin", &map);

    vector_t kvps;
    init_vector(&kvps, 10, sizeof(kvp_t));
    hashmap_convert_to_vector(map, &kvps);

    vector_t nodes;
    init_vector(&nodes, 10, sizeof(node_t));

    vector_t roots;
    init_vector(&roots, 10, sizeof(node_t*));
    int i;
    for (i = 0; i < kvps.size; i++)
    {
        node_t new;
        init_node(&new, vector_get(kvps, i));
        node_t* new_item = (node_t*)vector_push_back(&roots, (void*)&new);
        vector_push_back(&roots, new_item);
    }
    
    while (roots.size > 1)
    {
        int curr = roots.size - 1;
        void *item = vector_get(roots, curr);
        node_t *node = *(node_t**)item;
        kvp_t *kvp = (kvp_t*)node->ptr;
        printf("%p %p\n", node, kvp);
        vector_remove(&roots, curr);


        /*

        void *ap = vector_get(v, curr);
        void *bp = vector_get(v, curr - 1);

        tree_t tree;
        combine_tree(&tree, ap, bp);


        ap = vector_push_back(&v, ap);

        vector_remove(&v, curr);
        vector_remove(&v, curr - 1);

        node_t new;
        new.in_use = 1;
        new.key = 0;
        new.value = ((node_t *)ap)->value + ((node_t *)bp)->value;
        new.left = ap;
        new.right = bp;

        vector_push_back(&v, &new);
        vector_sort(v, comp2);
        */
    }

    /* kvp_t *root = (kvp_t *)vector_get(v, 0); */
    /* print_tree_rec(root, 0); */

    free_hashmap(&map);
    free_vector(nodes);
}