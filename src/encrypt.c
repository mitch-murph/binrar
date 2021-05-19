#include <stdio.h>
#include <string.h>
#include "encrypt.h"


#define SHIFT_AMOUNT 69

void shift_encrypt_bytes(FILE *fd1, FILE *fd2, int amount)
{
    int shiftCode;
    while ((shiftCode = fgetc(fd1)) != EOF)
    {
        shiftCode = shiftCode + amount;
        fputc(shiftCode, fd2);
    }
}

void shift_encrypt(FILE *fd1, FILE *fd2)
{
    shift_encrypt_bytes(fd1, fd2, SHIFT_AMOUNT);
}

void shift_decrypt(FILE *fd1, FILE *fd2)
{
    shift_encrypt_bytes(fd1, fd2, -SHIFT_AMOUNT);
}

void XOR_cipher(FILE *in_file, FILE *out_file, char *key)
{
    int i = 0;
    int keyLen = strlen(key);
    int buffer;
    while ((buffer = fgetc(in_file)) != EOF)
    {
        buffer = buffer ^ key[i++ % keyLen];
        fputc(buffer, out_file);
    }
    return 0;
}