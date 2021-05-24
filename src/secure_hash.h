#ifndef SECURE_HASH_H
#define SECURE_HASH_H

#define HASH_SIZE sizeof(long int) + 1

/*****************************************************************************
 * Public function prototypes
*****************************************************************************/
long int secureHashPasswordCheck(char *string, char *hash_string);
long int secureHashEncrypt(char *string, char *hash_string);

#endif