#include <stdio.h>
#include "shift_encrypt.h"

void shift_encrypt(char *input_file, char *output_file)
{
    int shiftCode;
    FILE *fd1, *fd2;

    fd1 = fopen(input_file, "rb");

    if (fd1 == NULL)
    {
        printf("\n doesn't exist");
    }
    else
    {
        fd2 = fopen(output_file, "wb");
        while ((shiftCode = fgetc(fd1)) != EOF)
        {
            shiftCode = shiftCode + 69;
            fputc(shiftCode, fd2);
        }
        fclose(fd2);
    }
    fclose(fd1);
    printf("File is encrypted. Encrypted file is:  %s.", output_file);
    printf("\n");
}

void shift_decrypt(char *input_file, char *output_file)
{

    int shiftCode;
    FILE *fd1, *fd2;

    fd1 = fopen(input_file, "rb");

    if (fd1 == NULL)
    {
        printf("\n doesn't exist");
    }
    else
    {
        fd2 = fopen(output_file, "wb");

        while ((shiftCode = fgetc(fd1)) != EOF)
        {

            shiftCode = shiftCode - 69;
            fputc(shiftCode, fd2);
        }
        fclose(fd2);
    }
    fclose(fd1);
    printf("File is decrypted. Decrypted file is:  %s.", output_file);
    printf("\n");
}
