#include "src/teacher_interface.h"
#include "src/compress.h"
#include <stdio.h>  /* printf */
#include <string.h> /* strcmp */

int main(int argc, char *argv[])
{
    if (argc == 2 && !strcmp(argv[1], "-t"))
    {
        teacherMainMenu();
    }
    else if (argc == 3 && !strcmp(argv[1], "-s"))
    {
        printf("Student Mode %s\n", argv[2]);
    }
    else
    {
        printf("Unknown run-time mode\n");
        printf("The run-time modes are\n");
        printf("Teacher: -t\n");
        printf("\t./binrar.out -t\n");
        printf("Student: -s <studentID>\n");
        printf("\t./binrar.out -s 101\n");
    }
    return 0;
}
