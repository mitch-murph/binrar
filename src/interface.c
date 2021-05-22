#include <stdio.h>
#include <string.h>
#include <math.h>
#include "interface.h"
#include "student.h"
#include "vector.h"
#include "search.h"
#include "database.h"

int scanDatabaseFilename(char *filename);
void wait_for_enter();
void launchWithDatabaseFile(char *database);
void mainMenu(void);
void studentMainMenu(student_t *student);
int scanMenu(void);
int scanStudentMenu(student_t *student);
void printMainMenu(void);
void printStudentMenu(student_t *student);
void addStudent(vector_t *student_list);
void deleteStudent(vector_t *student_list);
void displayStudent(const vector_t student_list);
void findStudent(const vector_t student_list);
void addAssessment(student_t *student);
void saveStudents(const vector_t student_list);
void loadStudents(vector_t *student_list);

void addTestData(vector_t *student_list)
{
    student_t new_student;
    initStudent(&new_student);
    new_student.studentId = 101;
    strcpy(new_student.firstName, "John");
    strcpy(new_student.lastName, "Smith");
    vector_push_back(student_list, &new_student);

    new_student.studentId = 102;
    initStudent(&new_student);
    strcpy(new_student.firstName, "Mary");
    strcpy(new_student.lastName, "Jane");
    vector_push_back(student_list, &new_student);

    new_student.studentId = 103;
    initStudent(&new_student);
    strcpy(new_student.firstName, "Mark");
    strcpy(new_student.lastName, "Lewis");
    vector_push_back(student_list, &new_student);

    new_student.studentId = 104;
    initStudent(&new_student);
    strcpy(new_student.firstName, "A very long nam");
    strcpy(new_student.lastName, "Lewis");
    vector_push_back(student_list, &new_student);
}

void launchTeacher()
{
    while (1)
    {
        printf("Welcome to the assessment database.\n"
               "Would you like to create a new database or use an existing?\n"
               "1. Use an existing.\n"
               "2. Create a new database.\n");
        printf("Enter your choice>");

        int option;
        scanf("%d", &option);
        /* Consume trailing newline */
        getchar();

        if (option == 1)
        {
            char filename[MAX_FILENAME_SIZE];
            if (!scanDatabaseFilename(filename))
            {
                launchWithDatabaseFile(filename);
                break;
            }
        }
        if (option == 2)
        {
            char filename[MAX_FILENAME_SIZE];
            if (!createNewDatabase(filename))
            {
                launchWithDatabaseFile(filename);
                break;
            }
        }
        printf("Invalid choice.\n");
    }
}

int scanDatabaseFilename(char *filename)
{
    printf("Enter the database file name>");
    int exceedLength = read_string_fix_length(filename,
                                              MAX_FILENAME_SIZE);
    if (exceedLength)
    {
        printf("\n\nFilename size exceed. Please rename the file.\n"
               "The max length is %d.\n"
               "You will be returned to the menu.\n",
               MAX_FILENAME_SIZE);
        wait_for_enter();
        return 1;
    }
    if (!checkIfFileExists(filename))
    {
        printf("\n\nCannot find file %s.\n"
               "You will be returned to the menu\n",
               filename);
        wait_for_enter();
        return 1;
    }
    return 0;
}

int createNewDatabase(char *filename)
{
    printf("Enter the new database file name>");
    int exceedLength = read_string_fix_length(filename,
                                              MAX_FILENAME_SIZE);
    if (exceedLength)
    {
        printf("\n\nFilename size exceed. Please rename the file.\n"
               "The max length is %d.\n"
               "You will be returned to the menu.\n",
               MAX_FILENAME_SIZE);
        wait_for_enter();
        return 1;
    }
    if (createEmptyDatabase(filename))
    {
        printf("\n\nCannot create database file %s.\n"
               "You will be returned to the menu\n",
               filename);
        wait_for_enter();
        return 1;
    }
    return 0;
}

void launchWithDatabaseFile(char *database)
{
}

void mainMenu(void)
{
    vector_t student_list;
    init_vector(&student_list, 10, sizeof(student_t));

    addTestData(&student_list);

    int choice;
    while ((choice = scanMenu()) != 6)
    {
        switch (choice)
        {
        case 1:
            addStudent(&student_list);
            break;
        case 2:
            deleteStudent(&student_list);
            break;
        case 3:
            displayStudent(student_list);
            break;
        case 4:
            findStudent(student_list);
            break;
        case 5:
            saveStudents(student_list);
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
           "3. display student list\n"
           "4. search for student\n"
           "5. save to database\n"
           "6. load database\n"
           "7. exit the program\n");
}

int read_string_fix_length(char *str, int length)
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

void wait_for_enter()
{
    printf("Press Enter to Continue");
    while (getchar() != '\n')
        ;
}

void addStudent(vector_t *student_list)
{
    student_t new_student;
    printf("Enter student ID>");
    scanf("%d", &new_student.studentId);
    /* Consume trailing newline */
    getchar();

    printf("Enter first name>");
    read_string_fix_length(new_student.firstName, MAX_NAME_SIZE);
    /* scanf("%[^\n]%*c", new_student.firstName); */

    printf("Enter last name>");
    read_string_fix_length(new_student.lastName, MAX_NAME_SIZE);
    /* scanf("%[^\n]%*c", new_student.lastName); */

    vector_push_back(student_list, &new_student);
}

void deleteStudent(vector_t *student_list)
{
}

void displayStudent(const vector_t student_list)
{
    int size = student_list.size;
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
            student_t *student = vector_get(student_list, size);
            printf("|%-15d", student->studentId);
            printf("|%-15s", student->firstName);
            printf("|%-15s|\n", student->lastName);
        }
        printf("-------------------------------------------------\n");
    }
    wait_for_enter();
}

void findStudent(const vector_t student_list)
{
    int studentId;
    printf("Enter the ID of the student>");
    scanf("%d", &studentId);
    /* Consume trailing newline */
    getchar();

    student_t *maybeStudent = searchStudent(student_list, studentId);
    if (maybeStudent == NULL)
    {
        printf("\n\nNo student found with ID %d\n", studentId);
        printf("You will you be returned to the menu.\n");
        wait_for_enter();
        return;
    }
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
            deleteStudent(&student_list);
            break;

        case 3:
            displayStudent(student_list);
            break;

        case 4:
            findStudent(student_list);
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
           "6. exit the program\n");
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
    read_string_fix_length(newAssessment.subject, MAX_NAME_SIZE);

    printf("Enter the assessment file name>");
    int exceedLength = read_string_fix_length(newAssessment.filename,
                                              MAX_FILENAME_SIZE);
    if (exceedLength)
    {
        printf("\n\nFilename size exceed. Please rename the file.\n"
               "The max length is %d.\n"
               "You will be returned to the menu.\n",
               MAX_FILENAME_SIZE);
        wait_for_enter();
        return;
    }
    if (!checkIfFileExists(newAssessment.filename))
    {
        printf("\n\nCannot find file %s.\n"
               "You will be returned to the menu\n",
               newAssessment.filename);
        wait_for_enter();
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

void saveStudents(const vector_t studentList)
{
    char filename[MAX_FILENAME_SIZE];
    printf("Enter the new database file name>");
    int exceedLength = read_string_fix_length(filename,
                                              MAX_FILENAME_SIZE);
    if (exceedLength)
    {
        printf("\n\nFilename size exceed. Please use a different name.\n"
               "The max length is %d.\n"
               "You will be returned to the menu.\n",
               MAX_FILENAME_SIZE);
        wait_for_enter();
        return;
    }

    char bitFlag = setBitFlag(filename);

    if (write_database(studentList, filename, bitFlag))
    {
        printf("\n\nCannot create database file %s.\n"
               "You will be returned to the menu\n",
               filename);
        wait_for_enter();
        return;
    }
}

void loadStudents(vector_t *student_list)
{
}