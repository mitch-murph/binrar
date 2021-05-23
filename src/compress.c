/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Compress
 * Functions for compressing files using huffman compression and run
 * length compression.
*****************************************************************************/
#include <stdio.h> /* printf, fputc, fgetc, fseek */
#include <string.h>
#include "compress.h"
#include "hashmap.h"
#include "vector.h"
#include "tree.h"
#include "bit_array.h"
#include "sort.h"
#include "search.h"

/*****************************************************************************
 * Private function prototypes
 *  - No private functions in this files
*****************************************************************************/
void countBytes(FILE *fp, hashmap_t *map);
int assignTreeAddrToNode(node_t *current, vector_t *addr);
int writeHuffmanTree(FILE *fp, node_t *root);
void hashmapConvert(hashmap_t hashmap, vector_t *vector);
node_t *buildHuffmanTree(vector_t nodes);
void writeCompressedFile(FILE *in_fp, FILE *out_fp, vector_t nodes);
void readCompressedFile(FILE *in_fp, FILE *out_fp, node_t *root,
                        int compressedSize);
void huffmanCompress(FILE *in_fp, FILE *out_fp);
void huffmanDecompress(FILE *in_fp, FILE *out_fp);

int comp(const void *a, const void *b)
{
    node_t *as = (node_t *)a;
    node_t *bs = (node_t *)b;
    return as->key == bs->key;
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

/*****************************************************************************
 * This function counts the frequency of each bytes in a file into a hashmap
 * Input:
 *   fp - File pointer to count the frequency of bytes from.
 *   map - Hashmap to record the frequencies into.
 * Output:
 *   map - This hashmap will contain the frequencies.
*****************************************************************************/
void countBytes(FILE *fp, hashmap_t *map)
{
    /* Read each char in the file until EOF is reached. */
    int buffer;
    while ((buffer = fgetc(fp)) != EOF)
    {
        /* Create a new node with the buffer and check the hashmap to 
           see if it already exists. */
        node_t node;
        node.key = buffer;
        node_t *maybeNode = hashmap_get(*map, &node);

        /* If it does exist increments its frequency count.
           Otherwises use node created. */
        if (maybeNode != NULL)
        {
            maybeNode->value++;
            hashmap_set(*map, maybeNode);
        }
        else
        {
            /* Initialise the new node and set it into the hashmap*/
            initNode(&node);
            node.in_use = 1;
            node.value++;
            hashmap_set(*map, &node);
        }
    }
}

/*****************************************************************************
 * This function transverses a tree determine the address of each node in
 * the tree. The address will result in a set of 1s and 0s. Where 1
 * denotes right and 0 is left.
 * Input:
 *   current - The current node being tranversed.
 *   addr - Vector of left and rights taken to reach current point.
 * Return:
 *   The sum of the node addresses in bits mutlipled by their frequency
 *   i.e. the size of the compressed file.
*****************************************************************************/
int assignTreeAddrToNode(node_t *current, vector_t *addr)
{
    if (current->left == NULL && current->right == NULL)
    {
        /* Multiple the frequency and size of the address
           to determine how much this byte will take up
           in the resulting compressed file. */
        int bitSize = current->value * addr->size;
        current->bitLength = addr->size - 1;

#ifdef DEBUG
        printf("%c freq: %d size: %d space: %d\n",
               current->key, current->value, addr->size, bitSize);
#endif

        /* Convert the address array to an int */
        current->value = convertVectorToBitArray(*addr);
        return bitSize;
    }
    else
    {
        int leftBitSize, rightBitSize;
        int temp;

        /* Tranvese the left side of the tree. Noting the direction 
           i.e. 0 */
        temp = 0;
        vectorPushBack(addr, &temp);
        leftBitSize = assignTreeAddrToNode(current->left, addr);
        vectorPop(addr);

        /* Tranvese the right side of the tree. Noting the direction 
           i.e. 1 */
        temp = 1;
        vectorPushBack(addr, &temp);
        rightBitSize = assignTreeAddrToNode(current->right, addr);
        vectorPop(addr);

        /* Return the sum of each side of the tree. */
        return leftBitSize + rightBitSize;
    }
}

/*****************************************************************************
 * This function tranverses a tree to deconstruct it into a readable form and
 * writes it to a file at the bit level.
 * Using a preorder depth first traversal i.e. (root, left, right).
 * Input:
 *   fp - The file pointer to write the tree to.
 *   root - The root node of the tree being written.
 * Return:
 *   The size of the huffman tree in bits i.e. the number of bits written.
*****************************************************************************/
int writeHuffmanTree(FILE *fp, node_t *root)
{
    /* Initialise a stack to be used for the depth first traversal. */
    vector_t stack;
    initVector(&stack, sizeof(node_t *));

    /* Push the root node into the stack to begin the traversal */
    vectorPushBack(&stack, &root);
    int buffer = 0, bufferSize = 0, size = 0;
    while (stack.size > 0)
    {
        /* Pop the stack to get the current node. */
        node_t *current = *(node_t **)vectorPop(&stack);

        /* If the current node is an end node */
        if (current->left == NULL && current->right == NULL)
        {
            /* write a 1 and the key of the node. */
            writeBit(&buffer, &bufferSize, 1, 1, fp);
            writeBit(&buffer, &bufferSize, current->key, 8, fp);
            size += 9;
        }
        else
        {
            /* If not end node, write a 0 and push 
               the left and right nodes onto the stack */
            writeBit(&buffer, &bufferSize, 0, 1, fp);
            size++;
            vectorPushBack(&stack, (void *)&current->left);
            vectorPushBack(&stack, (void *)&current->right);
        }
    }
    /* Flush the buffer out. 
       This will write any remaining bits in the buffer. */
    writeBit(&buffer, &bufferSize, 0, -1, fp);

    /* Return the size of the huffman tree in bits */
    return size;
}

/*****************************************************************************
 * This function converts the in use nodes of hashmap to a vector.
 * Input:
 *   hashmap - The hashmap to convert the nodes from.
 *   vector - Vector to push the nodes to.
 * Post:
 *   vector - The vector will now contain nodes from the hashmap
*****************************************************************************/
void hashmapConvert(hashmap_t hashmap, vector_t *vector)
{
    /* Loop through all possible positions of the hashmap */
    int i;
    for (i = 0; i < hashmap.map.capacity; i++)
    {
        /* Get the node at each position and check if it exists. */
        node_t *item = vectorGet(hashmap.map, i);
        if (hashmap.exists(item))
        {
            /* Copy the node to a new node
               and push it into the vector. */
            node_t *node = (node_t *)malloc(sizeof(node_t));
            initNodeFromNode(node, item);
            vectorPushBack(vector, &node);
        }
    }
}

/*****************************************************************************
 * This function builds a huffman tree using the frequencies of each node.
 * Input:
 *   nodes - Vector containing all the nodes.
 * Return:
 *   Pointer to the root node of the built huffman tree.
*****************************************************************************/
node_t *buildHuffmanTree(vector_t nodes)
{
    /* Loop while there is still more than 1 node vector. */
    while (nodes.size > 1)
    {
        /* Sort the nodes in the vector. */
        sort(nodes, compareNodeValue);
        int curr = nodes.size - 1;

        /* Get double pointers to the two least occurring nodes. */
        node_t **app = (node_t **)vectorGet(nodes, curr);
        node_t **bpp = (node_t **)vectorGet(nodes, curr - 1);

        /* Create a new node and set its children to
           the two least occurring nodes. 
           And the value is to be the sum of the children. */
        node_t *new = (node_t *)malloc(sizeof(node_t));
        new->left = *app;
        new->right = *bpp;
        new->in_use = 1;
        new->key = 0;
        new->value = (*app)->value + (*bpp)->value;

        /* Remove the two least occurring nodes. */
        vectorRemove(&nodes, curr);
        vectorRemove(&nodes, curr - 1);

        /* Push the new node onto the vector. */
        vectorPushBack(&nodes, &new);
    }
    /* Return the last remaining node in the vector .i.e. the root. */
    return *(node_t **)vectorGet(nodes, 0);
}

/*****************************************************************************
 * This function rewrites a file in its compressed form using the nodes
 * containing the address of each byte. 
 * Input:
 *   in_fp - The file to be read and compressed.
 *   out_fp - The file to write the compressed file.
 *   nodes - Vector of nodes containing the address of each byte.
 * Post:
 *   The out_fp file will have the compressed version of in_fp written.
*****************************************************************************/
void writeCompressedFile(FILE *in_fp, FILE *out_fp, vector_t nodes)
{
    /* Read each char in the file until EOF is reached. */
    int bitBuffer = 0, bitBufferSize = 0;
    int buffer;
    while ((buffer = fgetc(in_fp)) != EOF)
    {
        /* Find the node that has key matching the buffer */
        int index = linearSearch(nodes, compareNodeKey, &buffer);
        node_t *node = *(node_t **)vectorGet(nodes, index);

        /* Write the bit address of that node to the file. */
        writeBit(&bitBuffer, &bitBufferSize, node->value,
                 node->bitLength + 1, out_fp);
    }
    /* Flush the buffer out. 
       This will write any remaining bits in the buffer. */
    writeBit(&bitBuffer, &bitBufferSize, 0, -1, out_fp);
}

/*****************************************************************************
 * This function reads a compressed file and write it to a new file
 * uncompressed. 
 * Input:
 *   in_fp - The file to be read and decompressed.
 *   out_fp - The file to write the uncompressed file.
 *   root - The root node of the huffman tree to decode the compressed file.
 *   compressedSize = The compressed file size in bits. 
 * Post:
 *   The out_fp file will have the decompressed version of in_fp written.
*****************************************************************************/
void readCompressedFile(FILE *in_fp, FILE *out_fp, node_t *root,
                        int compressedSize)
{
    /* Loop over each bit in the compressed file. */
    node_t *curr = root;
    int buffer = 0;
    int i;
    for (i = 0; i < compressedSize; i++)
    {
        /* iBit is the current bit */
        int iBit = 7 - (i % 8);
        /* If the last bit has been reached, read the next char */
        if (iBit == 7)
            buffer = fgetc(in_fp);

        /* If the bit is a 1, transverse the right child. 
           Otherwise, the left. */
        int bit = getBit(buffer, iBit);
        if (bit)
            curr = curr->right;
        else
            curr = curr->left;

        /* If current node is an end node. */
        if (curr->right == NULL && curr->left == NULL)
        {
#ifdef DEBUG
            printf("%c", curr->key);
#endif

            /* Print the key of the current node.
               I.e. the decoded value of the bits read. */
            fputc(curr->key, out_fp);
            curr = root;
        }
    }
}

/*****************************************************************************
 * This function compresses a file by constructing a huffman tree and 
 * encoding the file. Writing both the tree and encoded version.
 * Input:
 *   in_fp - The file to be read and compressed.
 *   out_fp - The file to write the compressed file.
 * Post:
 *   The out_fp file will have the huffman tree and encoded compressed
 *   version written.
*****************************************************************************/
void huffmanCompress(FILE *in_fp, FILE *out_fp)
{
    /* Count the frequency of each byte into a hashmap. */
    hashmap_t map;
    init_hashmap(&map, sizeof(node_t), 0x101, comp, hash, exists, initNode);
    /* Store the start of the file so that file pointer can be reverted.
       NOTE we cannot seek the start incase the given in_fp is not
       at the start */
    long inFileStart = ftell(in_fp);
    countBytes(in_fp, &map);
    fseek(in_fp, inFileStart, SEEK_SET);

    /* Convert the hashmap into array of dynamically allocated tree nodes */
    vector_t nodes;
    initVector(&nodes, sizeof(node_t *));
    hashmapConvert(map, &nodes);
    free_hashmap(&map);

    /* Turn the vector of nodes into a huffman tree */
    vector_t nodesCopy;
    copyVector(&nodesCopy, nodes);
    node_t *root = buildHuffmanTree(nodesCopy);

    /* Tranverse the tree to find the address of each byte */
    vector_t addr;
    initVector(&addr, sizeof(int));
    int compressedSize = assignTreeAddrToNode(root, &addr);

#ifdef DEBUG
    printf("compressed size: %d\n", compressedSize);
#endif

    /* Write huffman tree */
    int treeSize = 0;
    fpos_t treeSizePos, treeEndPos;
    fgetpos(out_fp, &treeSizePos);
    fwrite(&treeSize, sizeof(int), 1, out_fp);

    treeSize = writeHuffmanTree(out_fp, root);
#ifdef DEBUG
    printf("huffman tree size: %d\n", treeSize);
#endif

    fgetpos(out_fp, &treeEndPos);
    fsetpos(out_fp, &treeSizePos);
    fwrite(&treeSize, sizeof(int), 1, out_fp);
    fsetpos(out_fp, &treeEndPos);

    /* Rewrite file subbing in addr for each byte */
    fwrite(&compressedSize, sizeof(int), 1, out_fp);
    writeCompressedFile(in_fp, out_fp, nodes);
    freeVector(nodes);
}

/*****************************************************************************
 * This function reads a huffman tree from a file. And constructs a tree
 * of nodes.
 * Input:
 *   fp - The file pointer containing the huffman tree.
 * Return:
 *   A pointer to the root node of the tree.
*****************************************************************************/
node_t *readHuffmanTree(FILE *fp)
{
    /* Read the tree size. */
    int treeSize;
    fread(&treeSize, sizeof(int), 1, fp);
#ifdef DEBUG
    printf("tree size: %d\n", treeSize);
#endif

    /* Initialise the stack to perform a depth first search
       while reading the tree. */
    vector_t stack;
    initVector(&stack, sizeof(node_t *));

    /* Perform the loop and stack with the root of the tree. */
    node_t *root = (node_t *)malloc(sizeof(node_t));
    vectorPushBack(&stack, &root);
    int buffer = 0, bufferSize = 0;
    while (stack.size > 0)
    {
        /* Pop the stack to get the current node. */
        node_t *curr = *(node_t **)vectorPop(&stack);

        /* If the current node is a 1, we have read an end node. */
        if (readBit(&buffer, &bufferSize, fp))
        {
            /* Read the next byte into the nodes key. */
            curr->key = readNBit(&buffer, &bufferSize, 8, fp);
#ifdef DEBUG
            printf("Read value: %c\n", curr->key);
#endif
        }
        else
        {
            /* Push the left and right nodes of onto the stack. */
            curr->left = (node_t *)malloc(sizeof(node_t));
            vectorPushBack(&stack, &curr->left);
            curr->right = (node_t *)malloc(sizeof(node_t));
            vectorPushBack(&stack, &curr->right);
        }
    }

    /* Return the root node of the tree. */
    return root;
}

/*****************************************************************************
 * This function decompresses a huffman encoded file into another file.
 * Input:
 *   in_fp - The file pointer containing to be decompressed.
 *   out_fp - The file pointer to be written to.
 * Post:
 *   The out_fp will contain the decompressed contents.
*****************************************************************************/
void huffmanDecompress(FILE *in_fp, FILE *out_fp)
{
    /* Read the huffman tree */
    node_t *root = readHuffmanTree(in_fp);

    /* Read the compressed file size */
    int compressedSize;
    fread(&compressedSize, sizeof(int), 1, in_fp);

#ifdef DEBUG
    printf("compressed size: %d\n", compressedSize);
#endif

    /* Decompress the file using the huffman tree read. */
    readCompressedFile(in_fp, out_fp, root, compressedSize);
}

/*****************************************************************************
 * This function compresses a file using run length compression
 * Input:
 *   in_fp - The file to be read and compressed.
 *   out_fp - The file to write the compressed file.
 * Post:
 *   The out_fp file will have the compressed version written.
*****************************************************************************/
void runLengthCompress(FILE *in_fp, FILE *out_fp)
{
    /* Loop over each byte in the file. */
    int flag = 0;
    int count = 1;
    int buffer, prev, temp;
    while ((buffer = fgetc(in_fp)) != EOF)
    {
        /* If its the first byte, there is no previous byte 
           to compare it with so it will just be put in temp. */
        if (flag == 0)
        {
            temp = buffer;
            flag = 1;
        }
        else
        {
            /* Rearrange the variables so that 
               prev contains the previous buffer
               and the current buffer will be in the prev next loop. */
            prev = temp;
            temp = buffer;
            if (prev == buffer)
            {
                /* If the previous matches the current, increment. */
                count++;
            }
            else
            {
                /* If it no longer matches the previous byte.
                   Write the count and the byte. 
                   Set count back to 0. */
                fputc(count, out_fp);
                fputc(prev, out_fp);
                count = 0;
            }
        }
    }
    /* Write the final byte and its size. Ensure the file is not empty. */
    if (flag != 0)
    {
        fputc(count, out_fp);
        fputc(prev, out_fp);
    }
}

/*****************************************************************************
 * This function decompresses a run length encoded file into another file.
 * Input:
 *   in_fp - The file pointer containing to be decompressed.
 *   out_fp - The file pointer to be written to.
 * Post:
 *   The out_fp will contain the decompressed contents.
*****************************************************************************/
void runLengthDecompress(FILE *in_fp, FILE *out_fp)
{
    /* Loop while not EOF */
    int frequency;
    while ((frequency = fgetc(in_fp)) != EOF)
    {
        /* Get the frequency and value */
        int value = fgetc(in_fp);

        /* Write the value, frequency times. */
        int i;
        for (i = 0; i < frequency; i++)
            fputc(value, out_fp);
    }
}