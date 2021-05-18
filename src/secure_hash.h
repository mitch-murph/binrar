#ifndef SECURE_HASH_H
#define SECURE_HASH_H

#define HASH_SIZE sizeof(long int) + 1

long int secure_hash_password_check(char *string, char *hash_string);
long int secure_hash_encrypt(char *string, char *hash_string);

#endif