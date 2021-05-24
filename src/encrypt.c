  
/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Encrypt
 * Functions for Encrypting the  files using:
    1. Shift encryption
    2. XOR Cipher
*****************************************************************************/

#include <stdio.h> /* fputc*/
#include <string.h> /* strlen*/
#include "encrypt.h" 


#define SHIFT_AMOUNT 69


/*****************************************************************************
 * This function opens the and gets the content from first file. If the file
 * is not empty, each bytes in the file is shifted by an amount and written 
 * in the second file.

 * Input:
 *   fp1 - File pointer to count the frequency of bytes from first file.

 * Output:
 *   fp2 - File pointer to write the shifted frequency of bytes to second 
            file.
*****************************************************************************/

void shift_encrypt_bytes(FILE *fd1, FILE *fd2, int amount)
{
    int shiftCode;
    while ((shiftCode = fgetc(fd1)) != EOF)
    {
        shiftCode = shiftCode + amount;
        fputc(shiftCode, fd2);
    }
}

/*****************************************************************************
 * This function encyrpts the file of diffrent types and shifts each 
 * bytes by an amount which correspondes to the ASCII characters

 * Input:
 *   input text: Hello everyone

 * Output:
 *   output text: z%yy# lel$(#&l
*****************************************************************************/

void shift_encrypt(FILE *fd1, FILE *fd2)
{
    /* encrypt the file to shift forward by the defined amount */ 
    shift_encrypt_bytes(fd1, fd2, SHIFT_AMOUNT);
}

/*****************************************************************************
 * This function dencyrpts the file the ASCII characters to the original
 * file. 

 * Input:
 *   input text: z%yy# lel$(#&l

 * Output:
 *   output text: Hello everyone
*****************************************************************************/

void shift_decrypt(FILE *fd1, FILE *fd2)
{
    /* decrypt the file to shifted backward by the defined amount 
        to get original text*/

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
}
