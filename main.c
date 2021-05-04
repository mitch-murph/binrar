#include "hashmap.h"
#include <stdio.h>
#define UNIT long int

void func(int pos, void *value)
{
    UNIT *c = (UNIT *)(value);
    printf("[%d] %ld\n", pos, *c);
}

struct s{
    int key;
    int value;
};

typedef struct s s_t;

int comp(const void *a, const void *b){
    s_t* as = (s_t*)a;
    s_t* bs = (s_t*)b;
    return as->key = bs->key;
}

int hash(const void *a){
    s_t* as = (s_t*)a;
    return as->key%100;
}

int main(int argc, char **argv)
{
    hashmap_t map;
    init_hashmap(&map, sizeof(struct s), comp, hash);

    s_t x;
    x.key = 10;
    x.value = 5;

    s_t y;
    y.key = 10;
    x.value = 55;

    set_hashmap(map, (void*)&x);
    s_t *z = (s_t*)get_hashmap(map, (void*)&y);

    printf("%d\n", z->value);
    free_hashmap(&map);
    return 0;
}