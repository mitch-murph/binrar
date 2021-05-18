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


    char key[255];
    printf("Enter password to encrypt with>");
    scanf("%s", key);

    Package(filenames, output_file);
}

int main(void)
{
    compress_test();
    return 0;
}