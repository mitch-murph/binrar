#include <string.h> /* strcpy */
#include <stdio.h>
#include <stdlib.h> /* atoi */
#include "interface.h"
#include "vector.h"
#include "database.h"
#include "student.h"

int loadStudentFromDatabase(vector_t *studentList, char *databaseFile);
int getStudentFromDatabase(vector_t studentList, student_t *student, char *studentId);
void startStudentMainMenu(student_t student);
void printStudentMainMenu(const void *a);
void myAssessmentList(student_t student);
void searchMyAssessments(student_t student);
void viewMyDetails(student_t student);
void printStudentMainList(const void *a);
void sortMyAssessments(student_t student);
void filterMyAssessments(student_t *tempStudent, const student_t student);

void studentMainMenu(char *databaseFile, char *studentId)
{
    vector_t studentList;
    initVector(&studentList, sizeof(student_t));
    if (loadStudentFromDatabase(&studentList, databaseFile))
    {
        freeVector(studentList);
        return;
    }

    student_t student;
    initStudent(&student);
    if (getStudentFromDatabase(studentList, &student, studentId))
    {
        freeVector(studentList);
        freeStudent(&student);
        return;
    }

    startStudentMainMenu(student);

    freeStudent(&student);
    freeVector(studentList);
}

int loadStudentFromDatabase(vector_t *studentList, char *databaseFile)
{
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

int getStudentFromDatabase(vector_t studentList, student_t *student, char *studentIdString)
{
    int studentId = atoi(studentIdString);
    int index = searchStudentIndex(studentList, studentId);
    if (index == -1)
    {
        printf("\n\nNo student found with ID %d\n", studentId);
        printf("The program will now exit.\n");
        waitForEnter();
        return 1;
    }

    *student = *(student_t *)vectorGet(studentList, index);
    return 0;
}

void startStudentMainMenu(student_t student)
{
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

void printStudentMainMenu(const void *a)
{
    printf("\n\nStudent Menu\n");
    printf("1. Open my assessment list\n"
           "2. Search for my assessments\n"
           "3. View my details\n"
           "4. Exit the program\n");
}

void myAssessmentList(student_t student)
{
    student_t tempStudent;
    copyStudent(&tempStudent, student);

    int choice;
    while ((choice = scanMenu(printStudentMainList, &tempStudent)) != 6 && choice != 1)
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

void printStudentMainList(const void *a)
{
    const student_t *student = a;
    printf("\n\nStudent Assessment List\n");
    displayStudentAssessments(*student);
    printf("1. Close my assessment list\n"
           "2. Search for my assessments\n"
           "3. View my details\n"
           "4. Sort by Mark\n"
           "5. Filter by subject\n"
           "6. Exit the program\n");
}

void searchMyAssessments(student_t student)
{
    char filename[MAX_FILENAME_SIZE];
    if (scanFilename(filename, "Enter the assessment file name>"))
        return;
    int index = searchStudentAssessmentIndex(student, filename);
    if (index == -1)
    {
        printf("\n\nNo assessment found with filename %s\n", filename);
        printf("You will you be returned to the menu.\n");
        waitForEnter();
    }

    assessment_t *assessment = vectorGet(student.assessments, index);
    printf("\n\n");
    printf("Filename: %s\n", assessment->filename);
    printf("Subject: %s\n", assessment->subject);
    printf("Mark: %d\n", assessment->mark);
    waitForEnter();
}

void viewMyDetails(student_t student)
{
    printf("\n\n");
    printf("Student ID: %d\n", student.studentId);
    printf("First Name: %s\n", student.firstName);
    printf("Last Name: %s\n", student.lastName);
    waitForEnter();
}

void filterMyAssessments(student_t *tempStudent, const student_t student)
{
    freeStudent(tempStudent);
    char subject[MAX_FILENAME_SIZE];
    printf("Please enter blank to remove filter.\n");
    printf("Enter subject name to filter by>");
    readStringFixLength(subject, MAX_NAME_SIZE);

    if (strcmp(subject, ""))
    {
        initStudent(tempStudent);
        int i;
        for (i = 0; i < student.assessments.size; i++)
        {
            assessment_t *assessment = vectorGet(student.assessments, i);
            if (!strcmp(assessment->subject, subject))
            {
                vectorPushBack(&tempStudent->assessments, assessment);
            }
        }
    }
    else
    {
        printf("Copying student %s\n", subject);
        copyStudent(tempStudent, student);
    }
}