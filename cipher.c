#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cipher.h"

int XORcipher(FILE *filep, char *key)
{
    int i;
    unsigned long int fileSize; /*holds number of bytes in file*/
    int keyLen = strlen(key);
    char *buffer = NULL;

    fseek(filep, 0, SEEK_END); /*Moves pointer to EOF*/
    fileSize = ftell(filep);   /*saves the location to find file size*/
    rewind(filep);             /*Moves point to beginning of file*/

    buffer = malloc(fileSize);

    if (fread(buffer, fileSize, 1, filep) != 1)
    {
        printf("Error reading file.\n");
        return -1;
    }

    /*XORcipher*/
    for (i = 0; i < fileSize; i++)
    {
        buffer[i] = buffer[i] ^ key[i % keyLen];
    }

    rewind(filep);

    /*Save encrypted data back into file*/
    if (fwrite(buffer, fileSize, 1, filep) != 1)
    {
        printf("Error in writing encrypted data to file.\n");
        return -1;
    }

    free(buffer);
    fclose(filep);

    return 0;
}
