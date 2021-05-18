#include "compress.h"
#include "bit_array.h"
#include "filepackager.h"
#include "cipher.h"
#include "shift_encrypt.h"
#include <stdio.h>
#include <string.h>

void printFill()
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n");
}

void print_filenames(int index, void *value)
{
    printf("[%d] %s\n", index, (char *)value);
}

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

    char key[255];
    printf("Enter password to encrypt with>");
    scanf("%s", key);

    FILE *fp1 = fopen(output_file, "rb");

    FILE *fp2 = fopen(output_file, "rb+");

    XORcipher(fp1, fp2, key);

    fclose(fp1);
    fclose(fp2);
}

int unpackage_test()
{
    char output_file[255];
    printf("Enter package filename>");
    scanf("%s", output_file);

    
    char key[255];
    printf("Enter password to encrypt with>");
    scanf("%s", key);

    FILE *fp1 = fopen(output_file, "rb");
    FILE *fp2 = fopen(output_file, "rb+");

    XORcipher(fp1, fp2, key);


    fclose(fp1);
    fclose(fp2);

    separate_files(output_file, "out/");
}

int main(void)
{
    compress_test();
    printf("Hit enter to unpackage");
    getchar();
    getchar();
    unpackage_test();

    return 0;
}