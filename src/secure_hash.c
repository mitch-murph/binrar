#include "secure_hash.h"
#include <string.h>

#define INITIAL_ENCRYPT 250285572184613
#define INITIAL_PASSWORD_CHECK 745558737037388
#define MAGIC 838474839873231

long int compute_hash(char *string, long int hash, long int magic)
{
    hash = hash * hash;
    int i;
    for (i = 0; i < strlen(string); i++)
    {
        hash = hash ^ (string[i]);
        hash = hash * magic;
    }
    return hash;
}

void hash_to_string(long int hash, char *hash_string)
{
    int i;
    for (i = 0; i < HASH_SIZE; i++)
    {
        unsigned char c = hash >> (i * 8);
        hash_string[i] = c;
    }
    hash_string[HASH_SIZE - 1] = '\0';
}

long int secure_hash_password_check(char *string, char *hash_string)
{
    long int result = compute_hash(string, INITIAL_PASSWORD_CHECK, MAGIC);
    hash_to_string(result, hash_string);
    return result;
}

long int secure_hash_encrypt(char *string, char *hash_string)
{
    long int result = compute_hash(string, INITIAL_ENCRYPT, MAGIC);
    hash_to_string(result, hash_string);
    return result;
}
