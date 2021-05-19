#include "src/compress.h"
#include "src/bit_array.h"
#include "src/filepackager.h"
#include "src/cipher.h"
#include "src/shift_encrypt.h"
#include "src/secure_hash.h"
#include "src/vector.h"
#include <stdio.h>
#include <string.h>

void printFill()
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n");
}

void print_filenames(int index, void *value);

void addFile(vector_t *filenames)
{
    char buffer[255] = "PLACEHOLDER";
    while (1)
    {
        printFill();
        print_vector(*filenames, print_filenames);
        printf("add file\n Enter filename> ");
        scanf("%s", buffer);
        if (!strcmp(buffer, "-1"))
            break;
        vector_push_back(filenames, buffer);
    }
}

void Package(vector_t filenames, char *output_file)
{
    printf("filenames:\n");
    print_vector(filenames, print_filenames);
    printf("output file: %s\n", output_file);
    combine_files(filenames, output_file);
}

void compress_test()
{
    vector_t filenames;
    init_vector(&filenames, 10, sizeof(char) * 255);
    addFile(&filenames);

    char output_file[255];
    printf("Enter output filename>");
    scanf("%s", output_file);

    Package(filenames, output_file);

    /*
    char key[255];
    printf("Enter password to encrypt with>");
    scanf("%s", key);

    FILE *fp1 = fopen(output_file, "rb");

    FILE *fp2 = fopen(output_file, "rb+");

    XORcipher(fp1, fp2, key);

    fclose(fp1);
    fclose(fp2);
    */
}

int unpackage_test()
{
    char output_file[255];
    printf("Enter package filename>");
    scanf("%s", output_file);

    /*
    char key[255];
    printf("Enter password to encrypt with>");
    scanf("%s", key);

    FILE *fp1 = fopen(output_file, "rb");
    FILE *fp2 = fopen(output_file, "rb+");

    XORcipher(fp1, fp2, key);


    fclose(fp1);
    fclose(fp2);
    */

    separate_files(output_file, "out/");
}

void password_hash_example()
{
    unsigned char out[HASH_SIZE];
    int i;
    secure_hash_password_check("B", out);
    for (i = 0; i < HASH_SIZE - 1; i++)
    {
        printf("%02x ", out[i]);
    }
    printf("\n");
    secure_hash_encrypt("B", out);
    for (i = 0; i < HASH_SIZE - 1; i++)
    {
        printf("%02x ", out[i]);
    }
    printf("\n%s ", out);
}

void compression_example()
{
    FILE *in_fp = fopen("1", "rb");
    FILE *out_fp = fopen("compressed", "wb");

    compress(in_fp, out_fp);

    fclose(in_fp);
    fclose(out_fp);

    in_fp = fopen("compressed", "rb");
    out_fp = fopen("uncompressed", "wb");

    decompress(in_fp, out_fp);

    fclose(in_fp);
    fclose(out_fp);
}

void encryption_example()
{
    printf("encryption_example\n");
    FILE *in_fp = fopen("data-files/opera.jpeg", "rb");
    if (in_fp == NULL)
    {
        printf("Error reading file\n");
    }
    FILE *out_fp = fopen("encrypted", "wb");
    if (out_fp == NULL)
    {
        printf("Error reading file\n");
    }

    char key[] = "password";

    XORcipher(in_fp, out_fp, key);

    fclose(in_fp);
    fclose(out_fp);

    in_fp = fopen("encrypted", "rb");
    out_fp = fopen("decrypted.png", "wb");

    XORcipher(in_fp, out_fp, key);

    fclose(in_fp);
    fclose(out_fp);
}

void example_from_class()
{
    compress_test();
    printf("Hit enter to unpackage");
    getchar();
    getchar();
    unpackage_test();
}

int main(void)
{
    encryption_example();
    return 0;
}