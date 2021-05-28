/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Encrypt
 * Functions for Encrypting the  files using:
    1. Shift encryption
    2. XOR Cipher
*****************************************************************************/
#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdio.h> /* fputc */

/*****************************************************************************
 * Public function prototypes
*****************************************************************************/
void shift_encrypt(FILE *fd1, FILE *fd2);
void shift_decrypt(FILE *fd1, FILE *fd2);
void XOR_cipher(FILE *in_file, FILE *out_file, char *key);

#endif
