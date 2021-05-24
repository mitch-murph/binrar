#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdio.h>

/*****************************************************************************
 * Public function prototypes
*****************************************************************************/
void huffmanCompress(FILE *in_fp, FILE *out_fp);
void huffmanDecompress(FILE *in_fp, FILE *out_fp);
void runLengthCompress(FILE *in_fp, FILE *out_fp);
void runLengthDecompress(FILE *in_fp, FILE *out_fp);

#endif