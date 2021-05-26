#include <stdio.h>
#include "vector.h"
#include "student.h"
#include "sort.h"

/*****************************************************************************
 * This function reads a string and only accepted a fixed length. Exceeding
 * characters are ignored.
 * Input:
 *   strp - the pointer to where the string is to be written to.
 *   length - the length of string to read.
 * Return:
 *   1 - input has exceeded the length.
 *   0 - input has not exceeded the length.
*****************************************************************************/
int readStringFixLength(char *strp, int length)
{
    int flag = 0;
    /* Read each character into *str by
	   iterating over each char in name.
	   Stop when length limit is reached 
       or '\n' is encountered. */
    while (--length > 0 && (*strp = getchar()) != '\n')
    {
        strp++;
    }

    /* Read the remaining characters that exceed 
	   length to clear the buffer.
	   Stop clearing if either the last character
	   entered is '\n' or until the next '\n'
	   is read. */
    while (*strp != '\n' && getchar() != '\n')
        flag = 1; /* Loop until '\n' reached */

    /* Set the last char in *str to the null-terminator */
    *strp = '\0';
    return flag;
}

int flushScan(void)
{
    int i = 0;
    while (getchar() != '\n')
        i++;
    return i;
}

int scanAreYouSure(char *message)
{
    char option;
    do
    {
        printf("%s", message);
        scanf("%c", &option);
        if (flushScan() == 0)
        {
            if (option == 'Y')
            {
                return 0;
            }
            else if (option == 'N')
            {
                return 1;
            }
        }
        printf("Invalid.\n"
               "Please only enter, Y for yes and N for no.\n");
    } while (1);
}

/*****************************************************************************
 * This function prints a message stating to press the enter key.
 * It then pauses the program until it is pressed.
 * Input:
 *   None
 * Return:
 *   None
*****************************************************************************/
void waitForEnter()
{
    printf("Press Enter to Continue");
    while (getchar() != '\n')
        ;
}

int scanMenu(void (*printMenu)(const void *a), const void *a)
{
    /* Print the menu */
    printMenu(a);
    /* Read user input from menu selection */
    printf("Enter your choice>");
    int option;
    scanf("%d", &option);
    /* Consume trailing newline */
    if (flushScan() != 0)
        return -1;
    return option;
}

void displayStudentAssessments(const student_t student)
{
    int size = student.assessments.size;
    printf("-------------------------------------------------\n");
    printf("|%-s|%-s|%s|\n",
           "Filename       ",
           "Subject        ",
           "Mark           ");
    printf("-------------------------------------------------\n");
    if (size < 1)
    {
        printf("|%-47s|\n",
               " No assesments.");
    }
    while (size--)
    {
        assessment_t *assessment = vectorGet(student.assessments, size);
        printf("|%15.15s", assessment->filename);
        printf("|%-15s", assessment->subject);
        printf("|%-15d|\n", assessment->mark);
    }
    printf("-------------------------------------------------\n");
}

void sortStudentAssessmentFile(student_t *student)
{
    printf("1. Ascending\n"
           "2. Descending\n");
    printf("Sort mark by>");
    int option;
    scanf("%d", &option);
    /* Consume newline */
    flushScan();
    if (option == 1)
    {
        sort(student->assessments, compareAssessmentAsc);
    }
    else if (option == 2)
    {
        sort(student->assessments, compareAssessmentDesc);
    }
    else
    {
        printf("Invalid.\n");
        printf("You will be returned to the menu list\n");
        waitForEnter();
    }
}

int scanFilename(char *filename, char *message)
{
    printf("%s", message);
    int exceedLength = readStringFixLength(filename,
                                           MAX_FILENAME_SIZE);
    if (exceedLength)
    {
        printf("\n\nFilename size exceed. Please rename the file.\n"
               "The max length is %d.\n"
               "You will be returned to the menu.\n",
               MAX_FILENAME_SIZE);
        waitForEnter();
        return 1;
    }
    return 0;
}