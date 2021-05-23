#ifndef DATABASE_H
#define DATABASE_H

#define XOR_ENCRYPT 1
#define SHIFT_ENCRYPT 2
#define HUFFMAN_COMPRESS 4
#define RUN_COMPRESS 8
#define MAX_FILENAME_SIZE 255

#include "vector.h"
#include <stdio.h>

struct file
{
    char filename[MAX_FILENAME_SIZE];
    long int size;
    char *bytes;
};

typedef struct file file_t;

int write_database(vector_t student_list, char *out_file, char bit_flag, vector_t existingFiles);
int read_database(char *database_file, vector_t *studentList);
int unpackage_database_files(char *database_file, char *dir);
int checkIfFileExists(char *filename);
int createEmptyDatabase(char *filename);
int read_database_to_memory(char *database_file, vector_t *files);

#endif