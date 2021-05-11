#include "compress.h"
#include "hashmap.h"
#include "vector.h"
#include <stdio.h>

/* PRIVATE */
typedef struct node node_t;

struct node
{
    int in_use;
    unsigned char key;
    int value;
    node_t *left;
    node_t *right;
};

void init_node(const void *a)
{
    node_t *node = (node_t *)a;
    node->in_use = 0;
    node->value = 0;
    node->left = NULL;
    node->right = NULL;
}

void init_node2(const void *a)
{
    node_t *node = (node_t *)a;
    node->in_use = 1;
    node->value = 0;
    node->left = NULL;
    node->right = NULL;
}

void count_bytes(char *filename, hashmap_t *map)
{
    int buffer;
    FILE *ptr;
    ptr = fopen(filename, "rb");

    while ((buffer = fgetc(ptr)) != EOF)
    {
        node_t node;
        node.key = buffer;

        node_t *node_maybe = (node_t *)hashmap_get(*map, (void *)&node);
        if (node_maybe != NULL)
        {
            node_maybe->value++;
            hashmap_set(*map, (void *)node_maybe);
        }
        else
        {
            init_node2(&node);
            node.value++;

            hashmap_set(*map, (void *)&node);
        }
    }

    fclose(ptr);
}

void func2(int pos, void *value)
{
    node_t *c = (node_t *)(value);
    printf("%p [%d]\t%c\t%d\n", value, pos, c->key, c->value);
}

int comp(const void *a, const void *b)
{
    node_t *as = (node_t *)a;
    node_t *bs = (node_t *)b;
    return as->key == bs->key;
}

int comp2(const void *a, const void *b)
{
    node_t *as = (node_t *)a;
    node_t *bs = (node_t *)b;
    return bs->value - as->value < 0;
}

int hash(const void *a)
{
    node_t *as = (node_t *)a;
    return as->key % 0x101;
}

int exists(const void *a)
{
    node_t *as = (node_t *)a;
    return as->in_use;
}

void print_tab(int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("  ");
    }
}

void print_tree_rec(node_t *n, int i)
{
    if (n == NULL)
    {
        return;
    }
    print_tab(i);
    printf("%p [%d] %02x\n", n, n->value, n->key);
    print_tab(i);
    printf("\t%p\t%p\n", n->left, n->right);
    print_tree_rec(n->left, i + 1);
    print_tree_rec(n->right, i + 1);
}

void tranverse(node_t *root)
{
    vector_t stack;
    init_vector(&stack, 10, sizeof(node_t *));
    printf("%p\n", root);
    vector_push_back(&stack, (void *)&root);

    node_t *jp = *(node_t **)vector_get(stack, 0);
    printf("%p\n", jp);
}

/* PUBLIC */
void compress()
{
    hashmap_t map;
    init_hashmap(&map, sizeof(node_t), 0x101, comp, hash, exists, init_node);
    count_bytes("data-files/comp.bin", &map);

    vector_t v;
    init_vector(&v, 10, sizeof(node_t));
    hashmap_convert_to_vector(map, &v);
    vector_sort(v, comp2);

    vector_t nodes;
    init_vector(&nodes, 1, sizeof(node_t));
    while (v.size > 1)
    {
        int curr = v.size - 1;
        node_t *ap = vector_get(v, curr);
        node_t *bp = vector_get(v, curr - 1);

        vector_push_back(&nodes, ap);
        int ai = nodes.size;
        vector_push_back(&nodes, bp);
        int bi = nodes.size;
        
        ap = vector_get(v, ai);
        bp = vector_get(v, bi);
        printf("%c\t%d\n", ap->key, ap->value);
        printf("%c\t%d\n", bp->key, bp->value);

        node_t new;
        new.in_use = 1;
        new.key = 0;
        new.value = ap->value + bp->value;
        new.left = ap;
        new.right = bp;

        printf("new value: %d\n", new.value);

        vector_remove(&v, curr);
        vector_remove(&v, curr - 1);

        vector_push_back(&v, &new);
        vector_sort(v, comp2);
    }
    print_vector(nodes, func2);

/*
    node_t *root = (node_t *)vector_get(v, 0);
    print_tree_rec(root, 0);
    printf("%d\n", root->value);
    tranverse(root);*/

    free_hashmap(&map);
    free_vector(v);
}