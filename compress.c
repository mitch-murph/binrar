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
    node_t *c = *(node_t **)(value);
    printf("%d\t%p [%d]\t%c\n", c->value, value, pos, c->key);
}

int comp(const void *a, const void *b)
{
    node_t *as = (node_t *)a;
    node_t *bs = (node_t *)b;
    return as->key == bs->key;
}

int comp2(const void *a, const void *b)
{
    node_t *as = *(node_t **)a;
    node_t *bs = *(node_t **)b;
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

void convert_bytes(node_t *current, vector_t *addr)
{
    if (current->left == NULL && current->right == NULL)
    {
        int i;
        for (i = 0; i < addr->size; i++)
        {
            int x =  *(int*)vector_get(*addr, i);
            printf("%d", x);
        }
        printf(" -> %02x\n", current->key);
    }
    else
    {
        int temp;

        temp = 0;
        vector_push_back(addr, &temp);
        convert_bytes(current->left, addr);
        vector_pop(addr);

        temp = 1;
        vector_push_back(addr, &temp);
        convert_bytes(current->right, addr);
        vector_pop(addr);
    }
}

void tranverse(node_t *root)
{
    FILE *fp;
    int buffer = 0;
    int temp = 0;
    int count = 0;
    int flag = 0;
    fp = fopen("data-files/write.bin", "wb");

    vector_t stack;
    init_vector(&stack, 10, sizeof(node_t *));

    vector_push_back(&stack, (void *)&root);
    while (stack.size > 0)
    {
        flag = 0;
        node_t *current = *(node_t **)vector_pop(&stack);
        if (current->left == NULL && current->right == NULL)
        {
            buffer = (buffer << 1) + 1;
            count++;
            int a = current->key >> count;
            buffer = buffer << 8 - count;
            temp = current->key << 8 - count;
            buffer = buffer | a;

            flag = 1;
        }
        else
        {
            buffer = buffer << 1;
            count++;
            vector_push_back(&stack, (void *)&current->left);
            vector_push_back(&stack, (void *)&current->right);
        }

        if (count > 7 || flag)
        {
            fputc((char)buffer, fp);
            if (flag)
            {
                buffer = temp >> 8 - count;
                if (count > 7)
                {
                    fputc((char)temp, fp);
                    count = 0;
                    buffer = 0;
                }
            }
            else
            {
                count = 0;
            }
        }
    }
    if (count > 0)
    {
        buffer = buffer << 8 - count;
        fputc((char)buffer, fp);
    }
    fclose(fp);
}

int hashmap_convert(hashmap_t hashmap, vector_t *vector)
{
    int i;
    for (i = 0; i < hashmap.map.capacity; i++)
    {
        node_t *item = (void *)vector_get(hashmap.map, i);
        node_t *node = (node_t *)malloc(sizeof(node_t));
        init_node(node);
        if (hashmap.exists(item))
        {
            node->in_use = item->in_use;
            node->key = item->key;
            node->left = item->left;
            node->right = item->right;
            node->value = item->value;
            vector_push_back(vector, &node);
        }
    }

    return 0;
}

/* PUBLIC */
void compress()
{
    hashmap_t map;
    init_hashmap(&map, sizeof(node_t), 0x101, comp, hash, exists, init_node);
    count_bytes("data-files/24-bit.bmp", &map);

    vector_t nodesp;
    init_vector(&nodesp, 10, sizeof(node_t *));
    hashmap_convert(map, &nodesp);
    vector_sort(nodesp, comp2);
    print_vector(nodesp, func2);

    while (nodesp.size > 1)
    {
        int curr = nodesp.size - 1;
        node_t **app = (node_t **)vector_get(nodesp, curr);
        node_t **bpp = (node_t **)vector_get(nodesp, curr - 1);

        node_t *new = (node_t *)malloc(sizeof(node_t));
        new->in_use = 1;
        new->key = 0;
        new->value = (*app)->value + (*bpp)->value;
        new->left = *app;
        new->right = *bpp;

        vector_remove(&nodesp, curr);
        vector_remove(&nodesp, curr - 1);

        vector_push_back(&nodesp, &new);
        vector_sort(nodesp, comp2);
    }
    node_t *head = *(node_t **)vector_get(nodesp, 0);
    tranverse(head);

    vector_t addr;
    init_vector(&addr, 10, sizeof(int));
    convert_bytes(head, &addr);

    free_hashmap(&map);
    free_vector(nodesp);
}