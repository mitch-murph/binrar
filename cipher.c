#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cipher.h"

int XORcipher(FILE *in_file, FILE *out_file, char *key)
{
    int i;
    int keyLen = strlen(key);
    char buffer;
    while ((buffer = fgetc(in_file)) != EOF)
    {
        buffer = buffer ^ key[i++ % keyLen];
        fputc(buffer, out_file);
    }
    return 0;
}
