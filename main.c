#include "hashmap.h"
#include "tree.h"
#include "vector.h"
#include <stdio.h>

typedef struct s s_t;

struct s
{
    int in_use;
    unsigned char key;
    int value;
    s_t *left;
    s_t *right;
};

void func(int pos, void *value)
{
    s_t *c = (s_t *)(value);
    printf("[%d] %02x %d\n", pos, c->key, c->value);
}

int comp(const void *a, const void *b)
{
    s_t *as = (s_t *)a;
    s_t *bs = (s_t *)b;
    return as->key = bs->key;
}

int comp2(const void *a, const void *b)
{
    s_t *as = (s_t *)a;
    s_t *bs = (s_t *)b;
    return bs->value - as->value < 0;
}

int hash(const void *a)
{
    s_t *as = (s_t *)a;
    return as->key % 0x101;
}

int exists(const void *a)
{
    s_t *as = (s_t *)a;
    return as->in_use;
}

void init(const void *a)
{
    s_t *as = (s_t *)a;
    as->in_use = 0;
    as->left = NULL;
    as->right = NULL;
}

void prepare_compression_map(hashmap_t *map)
{
    init_hashmap(map, sizeof(struct s), 0x101, comp, hash, exists, init);

    int buffer;
    FILE *ptr;

    ptr = fopen("data-files/image.png", "rb");

    while ((buffer = fgetc(ptr)) != EOF)
    {
        /*printf("%02x\n", buffer);*/
        s_t s;
        s.key = buffer;

        s_t *sMaybe = (s_t *)hashmap_get(*map, (void *)&s);
        if (sMaybe->in_use)
        {
            sMaybe->value++;
            hashmap_set(*map, (void *)sMaybe);
        }
        else
        {
            s.value = 1;
            s.in_use = 1;
            hashmap_set(*map, (void *)&s);
        }
    }

    fclose(ptr);
}

int main(int argc, char **argv)
{
    hashmap_t map;
    prepare_compression_map(&map);

    vector_t v;
    init_vector(&v, 10, sizeof(s_t));
    hashmap_convert_to_vector(map, &v);
    vector_sort(v, comp2);
    print_vector(v, func);

    vector_t nodes;
    init_vector(&nodes, v.size + 1, sizeof(s_t));

    while (v.size > 1)
    {
        int curr = v.size - 1;
        void *ap = vector_get(v, curr);
        void *bp = vector_get(v, curr - 1);

        ap = vector_push_back(&nodes, ap);
        bp = vector_push_back(&nodes, bp);

        vector_remove(&v, curr);
        vector_remove(&v, curr - 1);

        s_t new;
        new.in_use = 1;
        new.value = ((s_t *)ap)->value + ((s_t *)bp)->value;
        new.left = ap;
        new.right = bp;

        vector_push_back(&v, &new);
        vector_sort(v, comp2);
    }
    print_vector(v, func);

    free_hashmap(&map);
    free_vector(v);

    return 0;
}

int in_use;
unsigned char key;
int value;
s_t *left;
s_t *right;