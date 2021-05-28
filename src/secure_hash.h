/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Secure Hash
 * The implemenation of one way hash function
*****************************************************************************/
#ifndef SECURE_HASH_H
#define SECURE_HASH_H

/*****************************************************************************
 * Preprocessing directives (Public)
*****************************************************************************/
#define HASH_SIZE sizeof(long int) + 1 /* Size of a secure hash */

/*****************************************************************************
 * Public function prototypes
*****************************************************************************/
long int secureHashPasswordCheck(char *string, char *hash_string);
long int secureHashEncrypt(char *string, char *hash_string);

#endif