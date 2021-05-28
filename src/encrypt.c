/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Encrypt
 * Functions for Encrypting the  files using:
    1. Shift encryption
    2. XOR Cipher
*****************************************************************************/
#include <stdio.h>  /* fputc*/
#include <string.h> /* strlen*/
#include "encrypt.h"

#define SHIFT_AMOUNT 69

/*****************************************************************************
 * This function performs the actual shift encryption/decryption process.
 * Input:
 *   fd1 - The input file pointer.
 *   fd2 - The output file pointer
 * Post:
 *   The output file will contain the encrypted/decryption text.
*****************************************************************************/
void shift_encrypt_bytes(FILE *fd1, FILE *fd2, int amount)
{
    /* Loop over each byte and add the shift amount. */
    int shiftCode;
    while ((shiftCode = fgetc(fd1)) != EOF)
    {
        shiftCode = shiftCode + amount;
        fputc(shiftCode, fd2);
    }
}

/*****************************************************************************
 * This function encyrpts the file of diffrent types by shifting the ASCII 
 * number of original bytes by the defined shift amount and then the shifted
 * ASCII value is substituted as its corresponding characters. 
 * Input:
 *   fd1 - The input file pointer.
 *   fd2 - The output file pointer
 * Post:
 *   The output file will contain the encrypted text.
 * Example: 
 *   input character : A
 *   ASCII character of A: 65
 *   Adding shift_amount (69) to ASCII character of A: 134
 *   134 ASCII code to corresponding character : å
*****************************************************************************/
void shift_encrypt(FILE *fd1, FILE *fd2)
{
    /* encrypt the file by substituting with ASCII code by shifting 
       forward by the defined amount */
    shift_encrypt_bytes(fd1, fd2, SHIFT_AMOUNT);
}

/*****************************************************************************
 * This function dencyrpts the file the ASCII characters to the original
 * file. 
 * Input:
 *   fd1 - The input file pointer.
 *   fd2 - The output file pointer
 * Post:
 *   The output file will contain the encrypted text.
 * Example: 
 *   input character : å
 *   ASCII character of å : 134
 *   Subtracting shift_amount (69) to ASCII character of å: 65
 *   134 ASCII code to corresponding character : A
*****************************************************************************/
void shift_decrypt(FILE *fd1, FILE *fd2)
{
    /* decrypt the file to shifted backward by the defined amount 
        to get original text*/
    shift_encrypt_bytes(fd1, fd2, -SHIFT_AMOUNT);
}

/*****************************************************************************
 * This function encrypts the file using an XOR cipher, an additive cipher 
 * using a supplied key. This function is also used to decrypt the cipher.
 * Input:
 *   in_file - The input file pointer.
 *   out_file - The output file pointer
 *   key - The password to encrypt with
 * Post:
 *   The output file will contain the encrypted text.
 * Example: 
 *   input string : The quick brown fox jumps over the lazy dog
 *   input key : password
 *   ouput string : $	SADSARSDy
*****************************************************************************/
void XOR_cipher(FILE *in_file, FILE *out_file, char *key)
{
    int i = 0;
    int keyLen = strlen(key);
    int buffer;
    /* Loop over each byte and XOR the corresponding bytes of the key. */
    while ((buffer = fgetc(in_file)) != EOF)
    {
        buffer = buffer ^ key[i++ % keyLen];
        fputc(buffer, out_file);
    }
}
