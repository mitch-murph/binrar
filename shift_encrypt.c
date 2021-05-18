#include <stdio.h>
#include "shift_encrypt.h"

#define SHIFT_AMOUNT 69

void shift_encrypt(FILE *fd1, FILE *fd2)
{
    int shiftCode;
    while ((shiftCode = fgetc(fd1)) != EOF)
    {
        shiftCode = shiftCode + SHIFT_AMOUNT;
        fputc(shiftCode, fd2);
    }
}

void shift_decrypt(FILE *fd1, FILE *fd2)
{
    int shiftCode;
    while ((shiftCode = fgetc(fd1)) != EOF)
    {

        shiftCode = shiftCode - SHIFT_AMOUNT;
        fputc(shiftCode, fd2);
    }
}
