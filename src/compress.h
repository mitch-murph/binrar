/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Compress
 * Functions for compressing files using huffman compression and run
 * length compression.
*****************************************************************************/
#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdio.h> /* printf, fputc, fgetc, fseek, fgetpos, fsetpos */

/*****************************************************************************
 * Public function prototypes
*****************************************************************************/
void huffmanCompress(FILE *in_fp, FILE *out_fp);
void huffmanDecompress(FILE *in_fp, FILE *out_fp);
void runLengthCompress(FILE *in_fp, FILE *out_fp);
void runLengthDecompress(FILE *in_fp, FILE *out_fp);

#endif