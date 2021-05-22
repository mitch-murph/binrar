#ifndef DATABASE_H
#define DATABASE_H

#define XOR_ENCRYPT 1
#define SHIFT_ENCRYPT 2
#define HUFFMAN_COMPRESS 4
#define RUN_COMPRESS 8

#include "vector.h"
#include <stdio.h>

int write_database(vector_t filenames, char *out_file, char bit_flag);
int read_database(char *database_file, vector_t *filenames);
int unpackage_database_files(char *database_file, char *dir);
int checkIfFileExists(char *filename);

#endif