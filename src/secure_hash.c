#include <string.h> /* strlen */
#include "secure_hash.h"

#define INITIAL_ENCRYPT 250285572184613
#define INITIAL_PASSWORD_CHECK 745558737037388
#define MAGIC 838474839873231

/*****************************************************************************
 * Private function prototypes
*****************************************************************************/
long int computeHash(char *string, long int hash, long int magic);
void hashToString(long int hash, char *hash_string);

/*****************************************************************************
 * This function computes the hash for a given string.
 * Input:
 *   string - The string to be hashed.
 *   hash - The initial value of the hash.
 *   magic - The magic value to apply to the hash.
 * Return:
 *   hash - The resulting hash as a long int.
*****************************************************************************/
long int computeHash(char *string, long int hash, long int magic)
{
    /* Square the hash. */
    hash = hash * hash;

    /* Loop over each character in string and apply it
       and the magic to the hash. */
    int i;
    for (i = 0; i < strlen(string); i++)
    {
        hash = hash ^ (string[i]);
        hash = hash * magic;
    }

    /* Return the hash. */
    return hash;
}

/*****************************************************************************
 * This function converts a hash to a hash string.
 * Input:
 *   hash - The hash in long int form, that is to be convert.
 *   hashString - The string to insert the long int hash into.
 * Post:
 *   hashString - will now contain the hash as a string.
*****************************************************************************/
void hashToString(long int hash, char *hashString)
{
    /* Loop over each byte in the hash and put it into the hash string. */
    int i;
    for (i = 0; i < HASH_SIZE; i++)
    {
        unsigned char c = hash >> (i * 8);
        hashString[i] = c;
    }
    /* Set the last character to the null terminator. */
    hashString[HASH_SIZE - 1] = '\0';
}

/*****************************************************************************
 * This function computes the secure hash using the password check 
 * initial conditions.
 * Input:
 *   string - The string to be hashed.
 *   hashString - The string to insert the long int hash into.
 * Return:
 *   The hash as a long int.
*****************************************************************************/
long int secureHashPasswordCheck(char *string, char *hashString)
{
    /* Compute the hash using the password check initial conditions. */
    long int result = computeHash(string, INITIAL_PASSWORD_CHECK, MAGIC);
    /* Convert the hash into a string and insert it into hashString. */
    hashToString(result, hashString);
    return result;
}

/*****************************************************************************
 * This function computes the secure hash using the encryption initial 
 * conditions.
 * Input:
 *   string - The string to be hashed.
 *   hashString - The string to insert the long int hash into.
 * Return:
 *   The hash as a long int.
*****************************************************************************/
long int secureHashEncrypt(char *string, char *hashString)
{
    /* Compute the hash using encryption initial conditions. */
    long int result = computeHash(string, INITIAL_ENCRYPT, MAGIC);
    /* Convert the hash into a string and insert it into hashString. */
    hashToString(result, hashString);
    return result;
}
