/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Bit Array
 * Utility functions to work on the bit level. Includes reading and writing
 * to files as bits.
*****************************************************************************/
#include <stdio.h> /* printf, fputc, fgetc */
#include "bit_array.h"
#include "vector.h"

/*****************************************************************************
 * Private function prototypes
 *  - No private functions in this files
*****************************************************************************/

/*****************************************************************************
 * This function gets a bit in a given position.
 * Input:
 *   value - The value to retrieve the bit from.
 *   position - the position of the bit we what to retrieve.
 * Return:
 *   1 or 0 - The value of the bit.
*****************************************************************************/
int getBit(int value, int position)
{
    /* Bit shift a 1 to the wanted position and bitwise-and
       to check if bit exists */
    return (value & (1 << (position))) != 0;
}

/*****************************************************************************
 * This function prints all the bits in an integer
 * Input:
 *   value - The value to print the bits from.
 *   length - number of bits to be printed.
 * Post:
 *   The bit values will be printed to stdout.
*****************************************************************************/
void printBits(int value, int length)
{
    /* Loop through each bit, printing it until length reached. */
    int i;
    for (i = length; i >= 0; i--)
        printf("%d", getBit(value, i));
}

/*****************************************************************************
 * This function converts a vector of 1s and 0s, to a bit array.
 * Input:
 *   vector - The arry of 1s and 0s.
 * Return:
 *   bits - The bit arrary constructed by the vector.
*****************************************************************************/
int convertVectorToBitArray(vector_t vector)
{
    /* Loop through vector to retrieve each bit
       shifting it into bits until end is reached. */
    int i, bits = 0;
    for (i = 0; i < vector.size; i++)
    {
        int x = *(int *)vectorGet(vector, i);
        bits = (bits << 1) + x;
    }
    return bits;
}

/*****************************************************************************
 * This function buffers and writes bits to a file as a char.
 * Input:
 *   bufferp - Pointer to the buffer of bits
 *   buffer_sizep - Number of bits in the current buffer.
 *   value - The new bits to either write or store in the buffer.
 *   size - The number of new bits.
 *   filep - Pointer to the file to write the bits.
 * Post:
 *   Bits written to file if the buffer is full.
*****************************************************************************/
void writeBit(int *bufferp, int *bufferSizep,
              int value, int size, FILE *filep)
{
    /* If -1 is specificed we shift the entire buffer across
       so that the remaining can be written to the file. */
    if (size == -1)
    {
        value = 0;
        size = (8 - *bufferSizep);
    }

    /* Loop over the bits in value. */
    int i;
    for (i = size - 1; i >= 0; i--)
    {
        /* Shift each value into the buffer */
        *bufferp = (*bufferp << 1) + getBit(value, i);
        (*bufferSizep)++;

        /* If the buffer is full. Write it to the file. */
        if (*bufferSizep == 8)
        {
            #ifdef DEBUG
                printf("Writing bits %02x\n", *bufferp);
            #endif
            fputc(*bufferp, filep);
            *bufferp = 0;
            *bufferSizep = 0;
            #ifdef DEBUG
            #endif
        }
    }
}

/*****************************************************************************
 * This function buffers and reads bits from a file as a char.
 * Input:
 *   bufferp - Pointer to the buffer of bits
 *   buffer_sizep - Number of bits in the current buffer.
 *   filep - The file pointer to read bits from.
 * Return:
 *   The next bit in the buffer or file.
*****************************************************************************/
int readBit(int *bufferp, int *bufferSizep, FILE *filep)
{
    /* If the buffer is empty read the next char in the buffer
       Otherwise decrement the buffer size */
    if (*bufferSizep == 0)
    {
        *bufferp = fgetc(filep);
        *bufferSizep = 7;
        #ifdef DEBUG
            printf("Reading bits %02x\n", *bufferp);
        #endif
    }
    else
    {
        (*bufferSizep)--;
    }
    /* and return the next bit in the buffer. */
    return getBit(*bufferp, *bufferSizep);
}

/*****************************************************************************
 * This function buffers and reads a number of bits from a file as a char.
 * Input:
 *   bufferp - Pointer to the buffer of bits
 *   buffer_sizep - Number of bits in the current buffer.
 *   n - The number of bits to read.
 *   filep - The file pointer to read bits from.
 * Return:
 *   The next bit in the buffer or file.
*****************************************************************************/
int readNBit(int *bufferp, int *bufferSizep, int n, FILE *filep)
{
    /* Read n bits shifting them into temp.
       Return temp once n bits read. */
    int temp = 0;
    while (n--)
    {
        int a = readBit(bufferp, bufferSizep, filep);
        temp = (temp << 1) + a;
    }
    return temp;
}