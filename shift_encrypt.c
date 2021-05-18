#include <stdio.h>
#include "shift_encrypt.h"

void shift_encrypt(FILE *fd1, FILE *fd2)
{
    int shiftCode;
    while ((shiftCode = fgetc(fd1)) != EOF)
    {
        shiftCode = shiftCode + 69;
        fputc(shiftCode, fd2);
    }
}

void shift_decrypt(FILE *fd1, FILE *fd2)
{
    int shiftCode;
    while ((shiftCode = fgetc(fd1)) != EOF)
    {

        shiftCode = shiftCode - 69;
        fputc(shiftCode, fd2);
    }
}
