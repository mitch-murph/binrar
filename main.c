#include "compress.h"
#include "bit_array.h"
#include "filepackager.h"
#include "cipher.h"
#include "shift_encrypt.h"
#include <stdio.h>
#include <string.h>

void mainMenu();
void printFill();
void printMenu();
void addFile(vector_t *filenames);
void removeFile(vector_t *filenames);
void encDec();
void comDecomp();
void outputFile(char *output_file);
void Package(vector_t filenames, char *output_file);
void viewPackage();

int main(void)
{
    mainMenu();
    return 0;
}

void mainMenu()
{
    vector_t filenames;
    init_vector(&filenames, 10, sizeof(char) * 255);
    char output_file[255];

    int choice;
    while ((choice = scanMenu()) != 8)
    {
        switch (choice)
        {
        case 1:
            addFile(&filenames);
            break;

        case 2:
            removeFile(&filenames);
            break;

        case 3:
            encDec();
            break;

        case 4:
            comDecomp();
            break;

        case 5:
            outputFile(output_file);
            break;

        case 6:
            Package(filenames, output_file);
            break;

        case 7:
            viewPackage();
            break;

        default:
            printf("Invalid choice.\n");
            break;
        }
    }
}

int scanMenu()
{
    printFill();
    printMenu();
    int choice;
    scanf("%d", &choice);
    return choice;
}

void printFill()
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n");
}

void printMenu()
{
    printf("\n1. Add files\n"
           "2. Remove files\n"
           "3. Encrypt - XOR/Shift\n"
           "4. Compress \n"
           "5. Output file\n"
           "6. Package\n"
           "7. View Package\n"
           "8. EXIT\n"
           "Enter your choice>");
}

void print_filenames(int index, void *value)
{
    printf("[%d] %s\n", index, (char *)value);
}

void addFile(vector_t *filenames)
{
    getchar();
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
void removeFile(vector_t *filenames)
{
    getchar();
    int buffer;
    while (1)
    {
        printFill();
        print_vector(*filenames, print_filenames);
        printf("remove file\n Enter filename> ");
        scanf("%d", &buffer);
        if (buffer == -1)
            break;
        vector_remove(filenames, buffer);
    }
}
void encDec()
{
    int ch;
    printf("1.Encrypt\n  2.decrypt?\n Your choice>");
    scanf("%d", &ch);
    while (ch != 3)
    {
        switch (ch)
        {
        case 1:
            printf("1.XOR\n  2.shift?\n Your choice>");
            scanf("%d", &ch);

        case 2:
            printf("1.XOR\n  2.shift?\n Your choice>");
            scanf("%d", &ch);

        default:
            printf("ok");
        }
    }
}

void comDecomp()
{
}

void outputFile(char *output_file)
{
    printf("Enter output filename>");
    scanf("%s", output_file);
}
void viewPackage()
{
}
void Package(vector_t filenames, char *output_file)
{
    printf("filenames:\n");
    print_vector(filenames, print_filenames);
    printf("output file: %s\n", output_file);

}
