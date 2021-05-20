#include "src/compress.h"
#include "src/bit_array.h"
#include "src/database.h"
#include "src/encrypt.h"
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

void compress_test(int a)
{
    vector_t filenames;
    init_vector(&filenames, 10, sizeof(char) * 255);
    addFile(&filenames);

    char output_file[255];
    printf("Enter output filename>");
    scanf("%s", output_file);

    write_database(filenames, output_file, a);
}

int unpackage_test()
{
    char output_file[255];
    printf("Enter package filename>");
    scanf("%s", output_file);

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
    FILE *in_fp = fopen("ooo", "rb");
    FILE *out_fp = fopen("r", "wb");

    /* compress(in_fp, out_fp);

    fclose(in_fp);
    fclose(out_fp);

    in_fp = fopen("compressed", "rb");
    out_fp = fopen("uncompressed", "wb"); */

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

    XOR_cipher(in_fp, out_fp, key);

    fclose(in_fp);
    fclose(out_fp);

    in_fp = fopen("encrypted", "rb");
    out_fp = fopen("decrypted.png", "wb");

    XOR_cipher(in_fp, out_fp, key);

    fclose(in_fp);
    fclose(out_fp);
}

int main(void)
{
    compress_test(4);
    printf("compression completed\n");
    unpackage_database_files("out", "test");

    return 0;

    FILE *in_fp = fopen("out", "rb");
    FILE *out_fp = fopen("compressed", "wb");

    vector_t temp;
    init_vector(&temp, 10, sizeof(char) * 255);
    read_header(in_fp, &temp);
    char bit_flag;
    fread(&bit_flag, sizeof(char), 1, in_fp);
    compress(in_fp, out_fp);

    fclose(in_fp);
    fclose(out_fp);

    in_fp = fopen("compressed", "rb");
    out_fp = fopen("uncompressed", "wb");

    decompress(in_fp, out_fp);
    /* int buffer;
    int i = 19;
    while (i--)
    {
        buffer = fgetc(in_fp);
        print_bits_length(buffer, 7);
        printf("\n");
    }*/

    fclose(in_fp);
    fclose(out_fp);

    return 0;
}
