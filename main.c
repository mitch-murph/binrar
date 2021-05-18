#include "compress.h"
#include "bit_array.h"
#include "filepackager.h"
#include "cipher.h"
#include "shift_encrypt.h"
#include <stdio.h>

int main(void)
{
    mainMenu();
    return 0;
}

void mainMenu()
{
    char choice;
    printMenu();
    scanf("%d", &choice);

    while (choice != 8)
    {
        switch (choice)
        {
        case 1:
            addFile();
            break;

        case 2:
            removeFile();
            break;

        case 3:
            encDec();
            break;

        case 4:
            comDecomp();
            break;

        case 5:
            outputFile();
            break;

        case 6:
            unPackage();
            break;

        case 7:
            viewPackage();
            break;

        default:
            printf("Invalid choice.\n");
        }
        printMenu();
        scanf("%d", &choice);
    }
}
void printMenu()
{
    printf("\n1. Add files\n"
           "2. Remove files\n"
           "3. Encrypt - XOR/Shift\n"
           "4. Compress \n"
           "5. Output file\n"
           "6. Unpackage\n"
           "7. View Package\n"
           "8. EXIT\n"
           "Enter your choice>");
}
void addFile()
{
    printf("add file\n Enter filename> ");
    getchar();
    /* 1Add files
    - myfile1.txt
    - myfile2.txt
    - myfile3.txt
    */
}
void removeFile()
{
    printf("add file\n Enter filename> ");
    getchar();
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
    printf("add here");
    getchar();
}

void outputFile()
{
    printf("add here");
    getchar();
    /*
    4Output file
    Enter filename>
    */
}
void viewPackage()
{
    printf("add here");
    getchar();
}
void unPackage()
{
    printf("add here");
    getchar();
    /*
    6Unpackage
    Enter filename of the package>
    password>
    Enter output directory>
*/
}
