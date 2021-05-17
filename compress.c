#include "compress.h"
#include "hashmap.h"
#include "vector.h"
#include "tree.h"
#include "bit_array.h"
#include <stdio.h>

void count_bytes(char *filename, hashmap_t *map)
{
    int buffer;
    FILE *ptr;
    ptr = fopen(filename, "rb");

    while ((buffer = fgetc(ptr)) != EOF)
    {
        node_t node;
        node.key = buffer;

        node_t *maybe_node = (node_t *)hashmap_get(*map, (void *)&node);
        if (maybe_node != NULL)
        {
            maybe_node->value++;
            hashmap_set(*map, (void *)maybe_node);
        }
        else
        {
            init_node(&node);
            node.in_use = 1;
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

void assign_tree_addr_to_node(node_t *current, vector_t *addr)
{
    if (current->left == NULL && current->right == NULL)
    {
        current->bit_length = addr->size;
        current->value = convert_vector_to_bit_array(*addr);
    }
    else
    {
        int temp;

        temp = 0;
        vector_push_back(addr, &temp);
        assign_tree_addr_to_node(current->left, addr);
        vector_pop(addr);

        temp = 1;
        vector_push_back(addr, &temp);
        assign_tree_addr_to_node(current->right, addr);
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

void hashmap_convert(hashmap_t hashmap, vector_t *vector)
{
    int i;
    for (i = 0; i < hashmap.map.capacity; i++)
    {
        node_t *item = (void *)vector_get(hashmap.map, i);
        if (hashmap.exists(item))
        {
            node_t *node = (node_t *)malloc(sizeof(node_t));
            init_node(node);
            init_node_from_node(node, item);
            vector_push_back(vector, &node);
        }
    }
}

node_t *build_huffman_tree(vector_t nodes)
{
    while (nodes.size > 1)
    {
        vector_sort(nodes, comp2);
        int curr = nodes.size - 1;
        node_t **app = (node_t **)vector_get(nodes, curr);
        node_t **bpp = (node_t **)vector_get(nodes, curr - 1);

        node_t *new = (node_t *)malloc(sizeof(node_t));
        new->in_use = 1;
        new->key = 0;
        new->value = (*app)->value + (*bpp)->value;
        new->left = *app;
        new->right = *bpp;

        vector_remove(&nodes, curr);
        vector_remove(&nodes, curr - 1);

        vector_push_back(&nodes, &new);
    }
    return *(node_t **)vector_get(nodes, 0);
}

/* PUBLIC */
void compress(char *input_file, char *output_file)
{
    /* Count the frequency of each byte into a hashmap */
    hashmap_t map;
    init_hashmap(&map, sizeof(node_t), 0x101, comp, hash, exists, init_node);
    count_bytes(input_file, &map);

    /* Convert the hashmap into array of dynamically allocated tree nodes */
    vector_t nodes;
    init_vector(&nodes, 10, sizeof(node_t *));
    hashmap_convert(map, &nodes);
    free_hashmap(&map);

    /* Turn the vector of nodes into a huffman tree */
    vector_t nodes_copy;
    copy_vector(&nodes_copy, nodes);
    node_t *root = build_huffman_tree(nodes_copy);
    /* free_vector(nodes); */

    /* Tranverse the tree to find the address of each byte */
    vector_t addr;
    init_vector(&addr, 10, sizeof(int));
    assign_tree_addr_to_node(root, &addr);

    /*  */

    int i;
    for (i = 0; i < nodes.size; i++)
    {
        node_t *node = *(node_t **)vector_get(nodes, i);
        print_bits_length(node->value, node->bit_length);
        printf(" -> %02x\n", node->key);
    }

    /*
    tranverse(root);


    int i;
    for (i = 0; i < nodesp.size; i++)
    {
        node_t **node = (node_t **)vector_get(nodesp, i);
        free(*node);
    }
    */
}