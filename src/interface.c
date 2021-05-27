/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Interface
 * Functions that are common between student_interface and teacher_interface
*****************************************************************************/
#include "interface.h" /* Function declarations */

#include <stdio.h>   /* printf, getchar, scanf */
#include "vector.h"  /* vectorGet */
#include "student.h" /* compareAssessmentAsc, compareAssessmentDesc */
#include "sort.h"    /* sort */

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

/*****************************************************************************
 * This function flushs scan input by clearing all chars until newline.
 * Input:
 *   Node
 * Return:
 *   Number of characters cleared.
*****************************************************************************/
int flushScan(void)
{
    /* Read all chars until newline is reached. */
    int i = 0;
    while (getchar() != '\n')
        i++;
    /* Return number of chars read. */
    return i;
}

/*****************************************************************************
 * This function asks the user if they are sure they want to continue.
 * Input:
 *   message - The message to display to the user.
 * Return:
 *   0 - Yes, continue
 *   1 - No, dont continue
*****************************************************************************/
int scanAreYouSure(char *message)
{
    char option;
    do
    {
        /* Ask the users the message and read their input. */
        printf("%s", message);
        scanf("%c", &option);
        /* Check only one character was entered */
        if (flushScan() == 0)
        {
            /* Determine if it was Y or N, and return the result.
               If not, ask the user again. */
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

/*****************************************************************************
 * This function calls the provided printMenu function and then scans for 
 * the users option choice.
 * Input:
 *   printMenu - Pointer to the printMenu function.
 *   arg - the optional argument to pass to printMenu.
 * Return:
 *   The option selected.
 *   On read error will reutrn -1
*****************************************************************************/
int scanMenu(void (*printMenu)(const void *arg), const void *arg)
{
    /* Print the menu */
    printMenu(arg);
    /* Read user input from menu selection */
    printf("Enter your choice>");
    int option;
    scanf("%d", &option);
    /* Consume trailing newline */
    if (flushScan() != 0)
        return -1;
    return option;
}

/*****************************************************************************
 * This function prints a table of a students assessments.
 * Input:
 *   student - The students whos assignment is to be printed.
 * Return:
 *   None
*****************************************************************************/
void displayStudentAssessments(const student_t student)
{
    /* Print the table header. */
    printf("-------------------------------------------------\n");
    printf("|%-s|%-s|%s|\n",
           "Filename       ",
           "Subject        ",
           "Mark           ");
    printf("-------------------------------------------------\n");
    /* If there are no assessments. Print message. */
    int size = student.assessments.size;
    if (size < 1)
    {
        printf("|%-47s|\n",
               " No assesments.");
    }
    /* Loop over every assessment under the student. */
    while (size--)
    {
        /* Print the assessment. */
        assessment_t *assessment = vectorGet(student.assessments, size);
        printf("|%15.15s", assessment->filename);
        printf("|%-15s", assessment->subject);
        printf("|%-15d|\n", assessment->mark);
    }
    printf("-------------------------------------------------\n");
}

/*****************************************************************************
 * This function prints a table of a students assessments.
 * Input:
 *   student - The students whos assignment is to be printed.
 * Post:
 *   student assesments will now be sorted by either ascending 
 *   or descending order depending on option selected.
*****************************************************************************/
void sortStudentAssessmentFile(student_t *student)
{
    /* Ask the user if they want to sort ascending or descending. */
    printf("1. Ascending\n"
           "2. Descending\n");
    printf("Sort mark by>");
    int option;
    scanf("%d", &option);
    /* Consume newline */
    flushScan();

    /* Apply the option they have picked.
       If neither, return to menu. */
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

/*****************************************************************************
 * This function scans a filename from the user and ensure it is the correct
 * name size.
 * Input:
 *   filename - The variable to store the filename in.
 *   message - The message to display to the user before scanning the name.
 * Return:
 *   1 - Failure
 *   0 - Success
 * Post:
 *   filename will now contain the filename the user has entered.
*****************************************************************************/
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