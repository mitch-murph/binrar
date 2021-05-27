#include <string.h> /* strcpy */
#include <stdio.h>  /* printf */
#include <stdlib.h> /* atoi */
#include "interface.h"
#include "vector.h"
#include "database.h"
#include "student.h"

/*****************************************************************************
 * Private function prototypes
*****************************************************************************/
int loadStudentFromDatabase(vector_t *studentList, char *databaseFile);
int getStudentFromDatabase(vector_t studentList, student_t *student,
                           char *studentId);
void studentMainMenu(student_t student);
void printStudentMainMenu(const void *a);
void myAssessmentList(student_t student);
void searchMyAssessments(student_t student);
void viewMyDetails(student_t student);
void printStudentMainList(const void *a);
void sortMyAssessments(student_t student);
void filterMyAssessments(student_t *tempStudent, const student_t student);

/*****************************************************************************
 * This function is the main entry point to the student menu.
 * Input:
 *   databaseFile - The database file name.
 *   studentId - The student ID of the student launching the program.
 * Return:
 *   None
*****************************************************************************/
void startStudentMainMenu(char *databaseFile, char *studentId)
{
    /* Load in the students from the database. */
    vector_t studentList;
    initVector(&studentList, sizeof(student_t));
    if (loadStudentFromDatabase(&studentList, databaseFile))
    {
        freeVector(studentList);
        return;
    }

    /* Get the current student logged in from the student list. */
    student_t student;
    initStudent(&student);
    if (getStudentFromDatabase(studentList, &student, studentId))
    {
        freeVector(studentList);
        freeStudent(&student);
        return;
    }

    /* Start the student main menu */
    studentMainMenu(student);

    /* Clean up on program exit. */
    freeStudent(&student);
    freeVector(studentList);
}

/*****************************************************************************
 * This function loads the users from the database file.
 * Input:
 *   studentList - The vector to store the students retrieved from the 
 *                 database.
 *   databaseFile - The database file name.
 * Return:
 *   1 - Failure
 *   0 - Success
 * Post:
 *   studentList will now contain the students in the database.
*****************************************************************************/
int loadStudentFromDatabase(vector_t *studentList, char *databaseFile)
{
    /* Read the students from the database file. 
       If failure print a message and exit the program. */
    if (readDatabase(databaseFile, studentList))
    {
        printf("\n\nCannot read database file %s.\n"
               "The program will now exit.\n",
               databaseFile);
        waitForEnter();
        return 1;
    }
    printf("\n\nDatabase %s successfully loaded.\n",
           databaseFile);
    return 0;
}

/*****************************************************************************
 * This function gets the student information from a student ID. 
 * Input:
 *   studentList - The vector of students.
 *   student - student to insert the student information into. 
 *   studentIdString - The student ID to search for in string form.
 * Return:
 *   1 - Failure
 *   0 - Success
 * Post:
 *   student will not contain the student information if successful.
*****************************************************************************/
int getStudentFromDatabase(vector_t studentList, student_t *student,
                           char *studentIdString)
{
    /* Convert the student ID string into an integer. */
    int studentId = atoi(studentIdString);
    /* Look for that student ID in the student list */
    int index = searchStudentIndex(studentList, studentId);
    /* If not found print an error message
       And exit the program. */
    if (index == -1)
    {
        printf("\n\nNo student found with ID %d\n", studentId);
        printf("The program will now exit.\n");
        waitForEnter();
        return 1;
    }

    /* If the student is found, set it to student */
    *student = *(student_t *)vectorGet(studentList, index);
    return 0;
}

/*****************************************************************************
 * This function handles the interface for Student Main Menu. 
 * It calls the function to display the Student Main Menu and handle the 
 * input choice.
 * Input:
 *   student - All the student information. 
 * Post:
 *   Redirects to choice selected.
*****************************************************************************/
void studentMainMenu(student_t student)
{
    /* Loop over this menu until 4 is entered.
       When the user selects another menu they 
       will be redirected here on return back. */
    int choice;
    while ((choice = scanMenu(printStudentMainMenu, &student)) != 4)
    {
        switch (choice)
        {
        case 1:
            myAssessmentList(student);
            break;
        case 2:
            searchMyAssessments(student);
            break;
        case 3:
            viewMyDetails(student);
            break;
        default:
            printf("\nInvalid choice.\n");
        }
    }
}

/*****************************************************************************
 * This function prints the Student Main Menu Text
 * Input:
 *   arg - Unused parameter. Only listed to satisfy scanMenu()
 * Post:
 *   Student Main Menu text is printed to stdout
*****************************************************************************/
void printStudentMainMenu(const void *arg)
{
    printf("\n\nStudent Menu\n");
    printf("1. Open my assessment list\n"
           "2. Search for my assessments\n"
           "3. View my details\n"
           "4. Exit the program\n");
}

/*****************************************************************************
 * This function handles the interface for Student Assesment List. 
 * It calls the function to display the Student Main Menu with the assessment
 * list and and handles the input choice.
 * Input:
 *   student - All the student information. 
 * Post:
 *   Redirects to choice selected.
*****************************************************************************/
void myAssessmentList(student_t student)
{
    /* Create a temp copy of the student for filtering */
    student_t tempStudent;
    copyStudent(&tempStudent, student);

    /* Loop over this menu until 6 or 1 is entered.
       When the user selects another menu they 
       will be redirected here on return back. */
    int choice;
    while ((choice = scanMenu(printStudentMainList, &tempStudent)) != 6 &&
           choice != 1)
    {
        switch (choice)
        {
        case 2:
            searchMyAssessments(student);
            break;
        case 3:
            viewMyDetails(student);
            break;
        case 4:
            sortStudentAssessmentFile(&student);
            break;
        case 5:
            filterMyAssessments(&tempStudent, student);
            break;
        default:
            printf("\nInvalid choice.\n");
        }
    }
}

/*****************************************************************************
 * This function prints the Student Main Menu with assessment list Text
 * Input:
 *   arg - a void pointer to a student of student_t. Must be this type to
 *         satisfy scanMenu()
 * Post:
 *   Student Main Menu with assessment list text is printed to stdout
*****************************************************************************/
void printStudentMainList(const void *arg)
{
    const student_t *student = arg;
    printf("\n\nStudent Assessment List\n");
    displayStudentAssessments(*student);
    printf("1. Close my assessment list\n"
           "2. Search for my assessments\n"
           "3. View my details\n"
           "4. Sort by Mark\n"
           "5. Filter by subject\n"
           "6. Exit the program\n");
}

/*****************************************************************************
 * This function searches and displays a students assessment.
 * Input:
 *   student - The student who is to be queried for their assessments.
 * Post:
 *   Student assessment file name is scanned and if found the corresponding
 *   assessment is displayed.
*****************************************************************************/
void searchMyAssessments(student_t student)
{
    /* Ask the user to enter the assessment file name to search for. */
    char filename[MAX_FILENAME_SIZE];
    if (scanFilename(filename, "Enter the assessment file name>"))
        return;

    /* Search the student for it. 
    If not found, inform user and return to menu.  */
    int index = searchStudentAssessmentIndex(student, filename);
    if (index == -1)
    {
        printf("\n\nNo assessment found with filename %s\n", filename);
        printf("You will you be returned to the menu.\n");
        waitForEnter();
        return;
    }

    /* If found, display the assessment details to the user */
    assessment_t *assessment = vectorGet(student.assessments, index);
    printf("\n\n");
    printf("Filename: %s\n", assessment->filename);
    printf("Subject: %s\n", assessment->subject);
    printf("Mark: %d\n", assessment->mark);
    waitForEnter();
}

/*****************************************************************************
 * This function displays a students details.
 * Input:
 *   student - The student who is to be displayed.
 * Post:
 *   The student is printed to stdout.
*****************************************************************************/
void viewMyDetails(student_t student)
{
    /* Print student details and wait for enter key. */
    printf("\n\n");
    printf("Student ID: %d\n", student.studentId);
    printf("First Name: %s\n", student.firstName);
    printf("Last Name: %s\n", student.lastName);
    waitForEnter();
}

/*****************************************************************************
 * This function filters a students assessments by subject name.
 * Input:
 *   tempStudent - The variable of student which is used for displaying.
 *   student - All the student information. Not used for displaying.
 * Post:
 *   tempStudent will contain the filtered assessments.
*****************************************************************************/
void filterMyAssessments(student_t *tempStudent, const student_t student)
{
    /* Scan the subject to filter by. */
    freeStudent(tempStudent);
    char subject[MAX_NAME_SIZE];
    printf("Please enter blank to remove filter.\n");
    printf("Enter subject name to filter by>");
    readStringFixLength(subject, MAX_NAME_SIZE);

    if (strcmp(subject, ""))
    {
        /* If the user has not entered blank
           Loop over all the students assessments */
        initStudent(tempStudent);
        int i;
        for (i = 0; i < student.assessments.size; i++)
        {
            /* Compare each assessment subject name with the filter */
            assessment_t *assessment = vectorGet(student.assessments, i);
            if (!strcmp(assessment->subject, subject))
            {
                /* if they match append the assessment to the student. */
                vectorPushBack(&tempStudent->assessments, assessment);
            }
        }
    }
    else
    {
        /* If the user has entered blank
           add all assessments to the student to be displayed. */
        printf("Copying student %s\n", subject);
        copyStudent(tempStudent, student);
    }
}