#include "hashmap.h"
#include <stdio.h>
#define UNIT long int

void func(int pos, void *value)
{
    UNIT *c = (UNIT *)(value);
    printf("[%d] %ld\n", pos, *c);
}

struct s
{
    int in_use;
    unsigned char key;
    int value;
};

typedef struct s s_t;

int comp(const void *a, const void *b)
{
    s_t *as = (s_t *)a;
    s_t *bs = (s_t *)b;
    return as->key = bs->key;
}

int hash(const void *a)
{
    s_t *as = (s_t *)a;
    return as->key % 0x101;
}

void init(const void *a)
{
    s_t *as = (s_t *)a;
    as->in_use = 0;
}

void hash_demo()
{

    hashmap_t map;
    init_hashmap(&map, sizeof(struct s), comp, hash, init);

    int i = 0;
    for (i = 0; i < 10; i++)
    {
        s_t s;
        s.key = 'A' + i;
        s.value = i;
        s.in_use = 1;
        hashmap_set(map, (void *)&s);
    }

    for (i = 0; i < map.map.capacity; i++)
    {
        s_t *as = (s_t *)(vector_get(map.map, i));
        if (as->in_use)
        {
            printf("%p %c: %d\n", as, as->key, as->value);
        }
    }

    free_hashmap(&map);
}

int main(int argc, char **argv)
{
    hashmap_t map;
    init_hashmap(&map, sizeof(struct s), comp, hash, init);

    int buffer;
    FILE *ptr;

    ptr = fopen("image.png", "rb");

    while ((buffer = fgetc(ptr)) != EOF)
    {
        /*printf("%02x\n", buffer);*/
        s_t s;
        s.key = buffer;

        s_t *sMaybe = (s_t *)hashmap_get(map, (void *)&s);
        if (sMaybe->in_use)
        {
            sMaybe->value++;
            hashmap_set(map, (void *)sMaybe);
        }
        else
        {
            s.value = 1;
            s.in_use = 1;
            hashmap_set(map, (void *)&s);
        }
    }

    int i;
    for (i = 0; i < map.map.capacity; i++)
    {
        s_t *as = (s_t *)(vector_get(map.map, i));
        if (as->in_use)
        {
            printf("%p %02x: %d\n", as, as->key, as->value);
        }
    }

    return 0;
}