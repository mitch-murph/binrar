#include "src/teacher_interface.h" /* teacherMainMenu */
#include "src/student_interface.h" /* studentMainMenu */
#include <stdio.h>  /* printf */
#include <string.h> /* strcmp */

int main(int argc, char *argv[])
{
    if (argc == 2 && !strcmp(argv[1], "-t"))
    {
        teacherMainMenu();
    }
    else if (argc == 5 && !strcmp(argv[1], "-d") && !strcmp(argv[3], "-s"))
    {
        studentMainMenu( argv[2], argv[4]);
    }
    else
    {
        printf("Unknown run-time mode\n");
        printf("The run-time modes are\n");
        printf("Teacher: -t\n");
        printf("\t./binrar.out -t\n");
        printf("Student: -d <database> -s <studentID>\n");
        printf("\t./binrar.out -d student_database -s 101\n");
    }
    return 0;
}
