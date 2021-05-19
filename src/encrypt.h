#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdio.h>

void shift_encrypt(FILE *fd1, FILE *fd2);
void shift_decrypt(FILE *fd1, FILE *fd2);
void XOR_cipher(FILE *in_file, FILE *out_file, char *key);

#endif
