#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <stdio.h> /* printf, fputc, fgetc */
#include "vector.h"

int getBit(int value, int position);
void printBits(int value, int length);
int convertVectorToBitArray(vector_t vector);
void writeBit(int *bufferp, int *bufferSizep,
              int value, int size, FILE *filep);
int readBit(int *buffer, int *buffer_size, FILE *filep);
int readNBit(int *buffer, int *buffer_size, int n, FILE *filep);

#endif