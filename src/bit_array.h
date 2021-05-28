/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Bit Array
 * Utility functions to work on the bit level. Includes reading and writing
 * to files as bits.
*****************************************************************************/
#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <stdio.h> /* printf, fputc, fgetc */
#include "vector.h"

/*****************************************************************************
 * Public function prototypes
*****************************************************************************/
int getBit(int value, int position);
void printBits(int value, int length);
int convertVectorToBitArray(vector_t vector);
void writeBit(int *bufferp, int *bufferSizep,
              int value, int size, FILE *filep);
int readBit(int *buffer, int *bufferSize, FILE *filep);
int readNBit(int *buffer, int *bufferSize, int n, FILE *filep);

#endif