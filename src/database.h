#ifndef DATABASE_H
#define DATABASE_H

#include "vector.h"
#include <stdio.h>

/*****************************************************************************
 * Preprocessing directives
*****************************************************************************/
#define XOR_ENCRYPT 1         /* XOR encrypt bit flag position */
#define SHIFT_ENCRYPT 2       /* shift encrypt bit flag position */
#define HUFFMAN_COMPRESS 4    /* huffman compression bit flag position */
#define RUN_COMPRESS 8        /* run length compression bit flag position */
#define MAX_FILENAME_SIZE 255 /* Maximum filename size */

/*****************************************************************************
 * Structs
*****************************************************************************/
/* Struct to store information of a file. */
struct file
{
    char filename[MAX_FILENAME_SIZE];
    long int size;
    char *bytes;
};

typedef struct file file_t;

/*****************************************************************************
 * Public function prototypes
*****************************************************************************/
int checkIfFileExists(char *filename);
int checkIfFileExistsInDatabase(char *databaseFile, char *filename);
int writeDatabase(vector_t studentList, char *out_file, char bitFlag,
                  vector_t existingFiles);
int readDatabase(char *databaseFile, vector_t *studentList);
int readDatabaseToMemory(char *databaseFile, vector_t *files);
int unpackageDatabaseFiles(char *databaseFile, char *filename);

#endif