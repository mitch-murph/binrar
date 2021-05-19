#ifndef DATABASE_H
#define DATABASE_H

#define XOR_ENCRYPT 1
#define SHIFT_ENCRYPT 2
#define HUFFMAN_COMPRESS 4
#define RUN_COMPRESS 8

#include "vector.h"
#include <stdio.h>

int combine_files(vector_t filenames, char *out_file, char bit_flag);
int separate_files(char *in_file, char *dir);
void seek_file_contents(FILE *fp);
void copy_file(FILE *dest, FILE *src);

#endif