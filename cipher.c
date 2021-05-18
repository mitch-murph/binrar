#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int XORcipher(FILE *);
int cipher(void);

int cipher(void)
{
    FILE *fp;
    int encrypt;

    char fileName[25];

    printf("File to encrypt:");
    scanf("%s", fileName);

    fp = fopen(fileName, "rb+");

    if (fp == NULL)
    {
        printf("Failed to open file.\n");
        return -1;
    }

    encrypt = XORcipher(fp);

    return encrypt;
}
int XORcipher(FILE *filep)
{
    int i;
    unsigned long int fileSize; /*holds number of bytes in file*/
    int keyLen;
    char *buffer = NULL;
    char key[25];

    printf("Enter a password: ");
    scanf("%s", key);
    keyLen = strlen(key);

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
