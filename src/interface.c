#include <stdio.h>
#include <string.h>
#include <math.h>
#include "interface.h"
#include "student.h"
#include "vector.h"
#include "search.h"
#include "database.h"

void waitForEnter();
void mainMenu(void);
void studentMainMenu(student_t *student);
int scanMenu(void);
int scanStudentMenu(student_t *student);
void printMainMenu(void);
void printStudentMenu(student_t *student);
void addStudent(vector_t *studentList);
void deleteStudent(vector_t *studentList);
void displayStudent(const vector_t studentList);
void findStudent(const vector_t studentList);
void addAssessment(student_t *student);
void saveStudents(const vector_t studentList, char *databaseFile);
void loadStudents(vector_t *studentList, char *databaseFile);
void displayAllAssessments(vector_t studentList);
void displayAssessments(vector_t assessmentList);
void extractAllFiles(vector_t studentList, char *databaseFile);
void searchAllAssessments(vector_t assessmentList);

void mainMenu(void)
{
    vector_t studentList;
    init_vector(&studentList, 10, sizeof(student_t));
    char databaseFile[MAX_FILENAME_SIZE];
    databaseFile[0] = 0;

    int choice;
    while ((choice = scanMenu()) != 10)
    {
        switch (choice)
        {
        case 1:
            addStudent(&studentList);
            break;
        case 2:
            deleteStudent(&studentList);
            break;
        case 3:
            displayStudent(studentList);
            break;
        case 4:
            findStudent(studentList);
            break;
        case 5:
            displayAllAssessments(studentList);
            break;
        case 6:
            searchAllAssessments(studentList);
            break;
        case 7:
            saveStudents(studentList, databaseFile);
            break;
        case 8:
            loadStudents(&studentList, databaseFile);
            break;
        case 9:
            extractAllFiles(studentList, databaseFile);
            break;
        default:
            printf("Invalid choice.\n");
        }
    }
}

int scanMenu(void)
{
    /* Print the menu */
    printMainMenu();
    /* Read user input from menu selection */
    printf("Enter your choice>");
    int option;
    scanf("%d", &option);
    /* Consume trailing newline */
    getchar();
    return option;
}

void printMainMenu(void)
{
    printf("\n\n"
           "1. add student\n"
           "2. delete student\n"
           "3. display all students\n"
           "4. search for student\n"
           "5. display all assessments\n"
           "6. search for assessment\n"
           "7. save to database\n"
           "8. load database\n"
           "9. extract all files\n"
           "10. exit the program\n");
}

int readStringFixLength(char *str, int length)
{
    int flag = 0;
    /* Read each character into *str by
	   iterating over each char in name.
	   Stop when length limit is reached 
       or '\n' is encountered. */
    while (--length > 0 && (*str = getchar()) != '\n')
    {
        str++;
    }

    /* Read the remaining characters that exceed 
	   length to clear the buffer.
	   Stop clearing if either the last character
	   entered is '\n' or until the next '\n'
	   is read. */
    while (*str != '\n' && getchar() != '\n')
        flag = 1; /* Loop until '\n' reached */

    /* Set the last char in *str to the null-terminator */
    *str = '\0';
    return flag;
}

void waitForEnter()
{
    printf("Press Enter to Continue");
    while (getchar() != '\n')
        ;
}

void addStudent(vector_t *studentList)
{
    student_t new_student;
    printf("Enter student ID>");
    scanf("%d", &new_student.studentId);
    /* Consume trailing newline */
    getchar();

    printf("Enter first name>");
    readStringFixLength(new_student.firstName, MAX_NAME_SIZE);
    /* scanf("%[^\n]%*c", new_student.firstName); */

    printf("Enter last name>");
    readStringFixLength(new_student.lastName, MAX_NAME_SIZE);
    /* scanf("%[^\n]%*c", new_student.lastName); */

    vector_push_back(studentList, &new_student);
}

void deleteStudent(vector_t *studentList)
{
    int index = scanFindStudent(*studentList);
    if (index == -1)
        return;

    student_t *student = vector_get(*studentList, index);
    int studentId = student->studentId;
    vector_remove(studentList, index);
    printf("%d has been deleted.\n", studentId);
    waitForEnter();
}

void displayStudent(const vector_t studentList)
{
    int size = studentList.size;
    if (size < 1)
    {
        printf("No students.\n");
    }
    else
    {
        printf("-------------------------------------------------\n");
        printf("|%-s|%-s|%s|\n", "Student ID     ",
               "First Name     ",
               "Last Name      ");
        printf("-------------------------------------------------\n");
        while (size--)
        {
            student_t *student = vector_get(studentList, size);
            printf("|%-15d", student->studentId);
            printf("|%-15s", student->firstName);
            printf("|%-15s|\n", student->lastName);
        }
        printf("-------------------------------------------------\n");
    }
    waitForEnter();
}

int scanFindStudent(const vector_t studentList)
{
    int studentId;
    printf("Enter the ID of the student>");
    scanf("%d", &studentId);
    /* Consume trailing newline */
    getchar();

    int index = searchStudentIndex(studentList, studentId);
    if (index == -1)
    {
        printf("\n\nNo student found with ID %d\n", studentId);
        printf("You will you be returned to the menu.\n");
        waitForEnter();
    }
    return index;
}

void findStudent(const vector_t studentList)
{
    int index = scanFindStudent(studentList);
    if (index == -1)
        return;

    student_t *maybeStudent = vector_get(studentList, index);
    studentMainMenu(maybeStudent);
}

void studentMainMenu(student_t *student)
{
    int choice;
    while ((choice = scanStudentMenu(student)) != 6)
    {
        switch (choice)
        {
        case 1:
            addAssessment(student);
            break;

            /* case 2:
            deleteStudent(&studentList);
            break;

        case 3:
            displayStudent(studentList);
            break;

        case 4:
            findStudent(studentList);
            break; */

        default:
            printf("Invalid choice.\n");
        }
    }
}

void printStudentMenu(student_t *student)
{
    printf("\n\nStudent Menu\n");
    printf("For: %d - %s %s\n", student->studentId,
           student->firstName,
           student->lastName);

    printf("1. add assessment\n"
           "2. delete assessment\n"
           "3. display students assessments\n"
           "4. search for student\n"
           "5. read the student list from the database\n"
           "6. Return to main menu\n");
}

int scanStudentMenu(student_t *student)
{
    /* Print the menu */
    printStudentMenu(student);
    /* Read user input from menu selection */
    printf("Enter your choice>");
    int option;
    scanf("%d", &option);
    /* Consume trailing newline */
    getchar();
    return option;
}

void addAssessment(student_t *student)
{
    assessment_t newAssessment;

    printf("Enter subject name>");
    readStringFixLength(newAssessment.subject, MAX_NAME_SIZE);

    printf("Enter the assessment file name>");
    int exceedLength = readStringFixLength(newAssessment.filename,
                                              MAX_FILENAME_SIZE);
    if (exceedLength)
    {
        printf("\n\nFilename size exceed. Please rename the file.\n"
               "The max length is %d.\n"
               "You will be returned to the menu.\n",
               MAX_FILENAME_SIZE);
        waitForEnter();
        return;
    }
    if (!checkIfFileExists(newAssessment.filename))
    {
        printf("\n\nCannot find file %s.\n"
               "You will be returned to the menu\n",
               newAssessment.filename);
        waitForEnter();
        return;
    }

    printf("Enter the mark>");
    scanf("%d", &newAssessment.mark);
    /* Consume trailing newline */
    getchar();

    vector_push_back(&student->assessments, &newAssessment);
}

char setBitFlag(char *filename)
{
    char bitFlag = 0;
    while (1)
    {
        printf("\n\nDatabase file: %s\n", filename);
        printf("Please toggle the options you would\n"
               "like configured for your database.\n"
               "By selecting each number and then\n"
               "press 5 to execute.\n");
        printf("1. XOR Exncrypt: %d\n"
               "2. Shift Exncrypt: %d\n"
               "3. Huffman Compress: %d\n"
               "4. Run Length Compress: %d\n"
               "5. Execute\n",
               !!(bitFlag & XOR_ENCRYPT),
               !!(bitFlag & SHIFT_ENCRYPT),
               !!(bitFlag & HUFFMAN_COMPRESS),
               !!(bitFlag & RUN_COMPRESS));
        printf("Enter your choice>");
        int option;
        scanf("%d", &option);
        /* Consume trailing newline */
        getchar();
        if (0 < option && option <= 4)
            bitFlag = bitFlag | (char)pow(2, option - 1);
        else if (option == 5)
            return bitFlag;
        else
            printf("Invalid option.");
    }
}

void saveStudents(const vector_t studentList, char *databaseFile)
{
    vector_t existingFiles;
    init_vector(&existingFiles, 10, sizeof(file_t));
    if (databaseFile[0] != 0)
    {
        read_database_to_memory(databaseFile, &existingFiles);
    }
    int jj;
    for (jj = 0; jj < existingFiles.size; jj++)
    {
        file_t *file = vector_get(existingFiles, jj);
        printf("%s\n", file->filename);
    }

    char filename[MAX_FILENAME_SIZE];
    printf("Enter the new database file name>");
    int exceedLength = readStringFixLength(filename,
                                              MAX_FILENAME_SIZE);
    if (exceedLength)
    {
        printf("\n\nFilename size exceed. Please use a different name.\n"
               "The max length is %d.\n"
               "You will be returned to the menu.\n",
               MAX_FILENAME_SIZE);
        waitForEnter();
        return;
    }

    char bitFlag = setBitFlag(filename);

    if (write_database(studentList, filename, bitFlag, existingFiles))
    {
        printf("\n\nCannot create database file %s.\n"
               "You will be returned to the menu\n",
               filename);
        waitForEnter();
        return;
    }
}

void loadStudents(vector_t *studentList, char *filename)
{
    printf("Enter the database file name>");
    int exceedLength = readStringFixLength(filename,
                                              MAX_FILENAME_SIZE);
    if (exceedLength)
    {
        printf("\n\nFilename size exceed. Please rename the file.\n"
               "The max length is %d.\n"
               "You will be returned to the menu.\n",
               MAX_FILENAME_SIZE);
        waitForEnter();
        return;
    }
    if (read_database(filename, studentList))
    {
        printf("\n\nCannot read database file %s.\n"
               "You will be returned to the menu\n",
               filename);
        waitForEnter();
        return;
    }
}

void displayAllAssessments(vector_t studentList)
{
    vector_t assessments;
    getAllAssessments(studentList, &assessments);
    int size = assessments.size;
    if (size < 1)
    {
        printf("No assesments.\n");
    }
    else
    {
        printf("-------------------------------------------------\n");
        printf("|%-s|%-s|%s|\n", "Student ID     ",
               "Subject        ",
               "Mark           ");
        printf("-------------------------------------------------\n");
        while (size--)
        {
            assessment_student_t *assessment = vector_get(assessments, size);
            printf("|%-15d", assessment->studentp->studentId);
            printf("|%-15s", assessment->assessmentp->subject);
            printf("|%-15d|\n", assessment->assessmentp->mark);
        }
        printf("-------------------------------------------------\n");
    }
    waitForEnter();
    free_vector(assessments);
}

void extractAllFiles(vector_t studentList, char *databaseFile)
{
    char dir[MAX_FILENAME_SIZE];
    printf("Enter the directory you would like to extract to>");
    int exceedLength = readStringFixLength(dir,
                                              MAX_FILENAME_SIZE);
    if (exceedLength)
    {
        printf("\n\nDirectory name size exceed.\n"
               "The max length is %d.\n"
               "You will be returned to the menu.\n",
               MAX_FILENAME_SIZE);
        waitForEnter();
        return;
    }
    if (unpackage_database_files(databaseFile, dir))
    {
        printf("\n\nCannot read database file %s.\n"
               "You will be returned to the menu\n",
               databaseFile);
        waitForEnter();
        return;
    }
}

void searchAllAssessments(vector_t assessmentList)
{
}