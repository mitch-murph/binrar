  
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


/******************************************************************************
* This function reads content from file 1 to encrypt/decrypts and write it in file 2. 

* Input:
 *   read content in file 1

 * Output:
 *   write content in file 2.
 
******************************************************************************/

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
 * This function encyrpts the file of diffrent types by shifting the ASCII 
 * number of original bytes by the defined shift amount and then the shifted
 * ASCII value is substituted as its corresponding characters. 
 
 * Example: 
            input character : A
            ASCII character of A: 65
            Adding shift_amount (69) to ASCII character of A: 134
            134 ASCII code to corresponding character : å
 
        
 * Input:
 *   input text: Hello everyone

 * Output:
 *   output text: Mjqqt%j{jw~tsj
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
  
    * Example: 
            input character : å
            ASCII character of å : 134
            Subtracting shift_amount (69) to ASCII character of å: 65
            134 ASCII code to corresponding character : A

 * Input:
 *   input text: Mjqqt%j{jw~tsj

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
