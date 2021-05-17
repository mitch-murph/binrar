#ifndef FILEPACKAGER_H
#define FILEPACKAGER_H

#include "vector.h"
#include <stdio.h>

int combine_files(vector_t filenames, char* out_file);
int separate_files(char *in_file);
void seek_file_contents(FILE *fp);

#endif