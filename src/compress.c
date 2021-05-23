#include "compress.h"
#include "hashmap.h"
#include "vector.h"
#include "tree.h"
#include "bit_array.h"
#include "database.h"
#include "sort.h"
#include <stdio.h>
#include <string.h>

#define DEBUG

void count_bytes(FILE *fp, hashmap_t *map)
{
    int buffer;
    while ((buffer = fgetc(fp)) != EOF)
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

int assign_tree_addr_to_node(node_t *current, vector_t *addr)
{
    int left_bit_size, right_bit_size;
    if (current->left == NULL && current->right == NULL)
    {
        int bit_size = current->value * addr->size;
        current->bit_length = addr->size - 1;
#ifdef DEBUG
        printf("%c freq: %d size: %d space: %d\n", current->key, current->value, addr->size, bit_size);
#endif
        current->value = convert_vector_to_bit_array(*addr);
        return bit_size;
    }
    else
    {
        int temp;

        temp = 0;
        vectorPushBack(addr, &temp);
        left_bit_size = assign_tree_addr_to_node(current->left, addr);
        vectorPop(addr);

        temp = 1;
        vectorPushBack(addr, &temp);
        right_bit_size = assign_tree_addr_to_node(current->right, addr);
        vectorPop(addr);
    }
    return left_bit_size + right_bit_size;
}

int write_huffman_tree(FILE *fp, node_t *root)
{
    vector_t stack;
    initVector(&stack, sizeof(node_t *));

    vectorPushBack(&stack, (void *)&root);
    int buffer = 0, buffer_size = 0, size = 0;
    while (stack.size > 0)
    {
        node_t *current = *(node_t **)vectorPop(&stack);
        if (current->left == NULL && current->right == NULL)
        {
            write_bit(&buffer, &buffer_size, 1, 1, fp);
            write_bit(&buffer, &buffer_size, current->key, 8, fp);
            size += 9;
        }
        else
        {
            write_bit(&buffer, &buffer_size, 0, 1, fp);
            size++;
            vectorPushBack(&stack, (void *)&current->left);
            vectorPushBack(&stack, (void *)&current->right);
        }
    }
    write_bit(&buffer, &buffer_size, 0, -1, fp);

    return size;
}

void hashmap_convert(hashmap_t hashmap, vector_t *vector)
{
    int i;
    for (i = 0; i < hashmap.map.capacity; i++)
    {
        node_t *item = (void *)vectorGet(hashmap.map, i);
        if (hashmap.exists(item))
        {
            node_t *node = (node_t *)malloc(sizeof(node_t));
            init_node(node);
            init_node_from_node(node, item);
            vectorPushBack(vector, &node);
        }
    }
}

node_t *build_huffman_tree(vector_t nodes)
{
    while (nodes.size > 1)
    {
        sort(nodes, comp2);
        int curr = nodes.size - 1;
        node_t **app = (node_t **)vectorGet(nodes, curr);
        node_t **bpp = (node_t **)vectorGet(nodes, curr - 1);

        node_t *new = (node_t *)malloc(sizeof(node_t));
        new->in_use = 1;
        new->key = 0;
        new->value = (*app)->value + (*bpp)->value;
        new->left = *app;
        new->right = *bpp;

        vectorRemove(&nodes, curr);
        vectorRemove(&nodes, curr - 1);

        vectorPushBack(&nodes, &new);
    }
    return *(node_t **)vectorGet(nodes, 0);
}

void write_compressed_file(FILE *in_fp, FILE *out_fp, vector_t nodes)
{
    int i;
    for (i = 0; i < nodes.size; i++)
    {
        node_t *node = *(node_t **)vectorGet(nodes, i);
#ifdef DEBUG
        print_bits_length(node->value, node->bit_length);
        printf(" -> %c\n", node->key);
#endif
    }

    int bit_buffer = 0, bit_buffer_size = 0;
    int buffer;
    while ((buffer = fgetc(in_fp)) != EOF)
    {
        int bits = 0;
        int bit_length = 0;
        int i;
        for (i = 0; i < nodes.size; i++)
        {
            node_t *node = *(node_t **)vectorGet(nodes, i);
            if (node->key == buffer)
            {
                bits = node->value;
                bit_length = node->bit_length;
                break;
            }
        }
        write_bit(&bit_buffer, &bit_buffer_size, bits, bit_length + 1, out_fp);
    }
    write_bit(&bit_buffer, &bit_buffer_size, 0, -1, out_fp);
}

void read_compressed_file(FILE *in_fp, FILE *out_fp, node_t *root, int compressed_size)
{
    node_t *curr = root;

    int buffer = 0;
    int i;
    for (i = 0; i < compressed_size; i++)
    {
        int i_bit = 7 - (i % 8);
        if (i_bit == 7)
            buffer = fgetc(in_fp);

        int bit = get_bit(buffer, i_bit);
        if (bit)
            curr = curr->right;
        else
            curr = curr->left;

        if (curr->right == NULL && curr->left == NULL)
        {
            fputc(curr->key, out_fp);
            #ifdef DEBUG
                printf("%c", curr->key);
            #endif
            curr = root;
        }
    }
}

/* PUBLIC */
void compress(FILE *in_fp, FILE *out_fp)
{
    /* Count the frequency of each byte into a hashmap */
    hashmap_t map;
    init_hashmap(&map, sizeof(node_t), 0x101, comp, hash, exists, init_node);
    long in_file_start = ftell(in_fp);
    count_bytes(in_fp, &map);
    fseek(in_fp, in_file_start, SEEK_SET);

    /* Convert the hashmap into array of dynamically allocated tree nodes */
    vector_t nodes;
    initVector(&nodes, sizeof(node_t *));
    hashmap_convert(map, &nodes);
    free_hashmap(&map);

    /* Turn the vector of nodes into a huffman tree */
    vector_t nodes_copy;
    copyVector(&nodes_copy, nodes);
    node_t *root = build_huffman_tree(nodes_copy);
    /* freeVector(nodes); */

    /* Tranverse the tree to find the address of each byte */
    vector_t addr;
    initVector(&addr, sizeof(int));
    int compressed_size = assign_tree_addr_to_node(root, &addr);
#ifdef DEBUG
    printf("compressed size: %d = %d %d/8\n", compressed_size, compressed_size / 8, compressed_size % 8);
#endif

    /* Write huffman tree */
    int tree_size = 0;
    fpos_t tree_size_pos, tree_end_pos;
    fgetpos(out_fp, &tree_size_pos);
    fwrite(&tree_size, sizeof(int), 1, out_fp);

    tree_size = write_huffman_tree(out_fp, root);
#ifdef DEBUG
    printf("huffman tree size: %d = %d %d/8\n", tree_size, tree_size / 8, tree_size % 8);
#endif

    fgetpos(out_fp, &tree_end_pos);
    fsetpos(out_fp, &tree_size_pos);
    fwrite(&tree_size, sizeof(int), 1, out_fp);
    fsetpos(out_fp, &tree_end_pos);

    /* Rewrite file subbing in addr for each byte */
    fwrite(&compressed_size, sizeof(int), 1, out_fp);
    write_compressed_file(in_fp, out_fp, nodes);
}

node_t *read_huffman_tree(FILE *fp)
{
    int tree_size;
    fread(&tree_size, sizeof(int), 1, fp);
    printf("tree size: %d\n", tree_size);

    vector_t stack;
    initVector(&stack, sizeof(node_t *));

    node_t *root = (node_t *)malloc(sizeof(node_t));
    vectorPushBack(&stack, &root);
    int buffer = 0, buffer_size = 0;
    while (stack.size > 0)
    {
        node_t *curr = *(node_t **)vectorPop(&stack);
        if (read_bit(&buffer, &buffer_size, fp))
        {
            curr->key = read_n_bit(&buffer, &buffer_size, 8, fp);
            printf("Read value: %c\n", curr->key);
        }
        else
        {
            curr->left = (node_t *)malloc(sizeof(node_t));
            vectorPushBack(&stack, &curr->left);
            curr->right = (node_t *)malloc(sizeof(node_t));
            vectorPushBack(&stack, &curr->right);
        }
    }

    return root;
}

void decompress(FILE *in_fp, FILE *out_fp)
{
    /* Read the tree */
    node_t *root = read_huffman_tree(in_fp);
    int compressed_size;
    fread(&compressed_size, sizeof(int), 1, in_fp);
    printf("compressed size: %d\n", compressed_size);
    read_compressed_file(in_fp, out_fp, root, compressed_size);
}