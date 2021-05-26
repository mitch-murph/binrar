#include <string.h> /* strcpy */
#include <stdio.h>  /* printf, scanf */
#include <math.h>   /* pow */
#include "teacher_interface.h"
#include "database.h"
#include "vector.h"
#include "student.h"
#include "sort.h"

int readStringFixLength(char *strp, int length);
int flushScan(void);
int scanAreYouSure(char *message);
int scanFilename(char *filename, char *message);
void teacherMainMenu(void);
int scanMenu(void (*printMenu)(const void *a), const void *);
int scanFindStudent(const vector_t studentList);
void printLaunch(void);
void printTeacherMainMenu(const void *a);

void teacherStudentMainMenu(vector_t *studentList, char *databaseFile);
void teacherAssessmentMenu(vector_t *studentList, char *databaseFile);
char setBitFlag(char *filename);
void saveDatabase(const vector_t studentList, char *databaseFile);
void loadDatabase(vector_t *studentList, char *databaseFile);
void extractAll(char *databaseFile);

void printTeacherStudentMainMenu(const void *studentList);
void displayAllStudents(vector_t *studentList);
void findStudent(const vector_t studentList, char *databaseFile);
void addStudent(vector_t *studentList);
void deleteStudent(vector_t *studentList);
void teacherStudentListMenu(vector_t *studentList, char *databaseFile);
void printTeacherStudentListMenu(const void *studentList);
void sortStudents(vector_t *studentList);

void printTeacherAssessmentMenu(const void *a);
void teacherAssessmentsListMenu(vector_t *studentList, char *databaseFile);
void displayAllAssessments(vector_t *studentList);
void addAssessment(vector_t *studentList);
void deleteAssessment(vector_t *studentList);
void extractAssessmentFile(char *databaseFile);
void sortAllAssessmentList(vector_t *assessments);
void filterAllAssessmentList(const vector_t studentList, vector_t *assessments);
void printTeacherAssessmentListMenu(const void *a);

void teacherStudentMenu(student_t *student, char *databaseFile);
void printTeacherStudentMenu(const void *a);
void studentAssessmentList(student_t *student, char *databaseFile);
void addStudentAssessments(student_t *student);
void deleteStudentAssessment(student_t *student);
void extractStudentAssessmentFile(student_t *student, char *databaseFile);

void displayStudentAssessments(const student_t student);
void printStudentAssessmentList(const void *a);

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

void teacherMainMenu(void)
{
    vector_t studentList;
    initVector(&studentList, sizeof(student_t));
    char databaseFile[MAX_FILENAME_SIZE];
    databaseFile[0] = 0;

    printLaunch();

    int choice;
    while ((choice = scanMenu(printTeacherMainMenu, NULL)) != 7)
    {
        switch (choice)
        {
        case 1:
            teacherStudentMainMenu(&studentList, databaseFile);
            break;
        case 2:
            teacherAssessmentMenu(&studentList, databaseFile);
            break;
        case 3:
            saveDatabase(studentList, databaseFile);
            break;
        case 4:
            loadDatabase(&studentList, databaseFile);
            break;
        case 5:
            extractAll(databaseFile);
            break;
        default:
            printf("\nInvalid choice.\n");
        }
    }
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

void printLaunch(void)
{
    printf("\n\n"
           "Welcome to student assessment file management\n");
}

void printTeacherMainMenu(const void *a)
{
    printf("\n\nMain Menu\n");
    printf("1. Student menu\n"
           "2. Assessment menu\n"
           "3. Save to database\n"
           "4. Load database\n"
           "5. Extract all files\n"
           "6. View help\n"
           "7. Exit the program\n");
}

void teacherStudentMainMenu(vector_t *studentList, char *databaseFile)
{
    int choice;
    while ((choice = scanMenu(printTeacherStudentMainMenu, NULL)) != 5)
    {
        switch (choice)
        {
        case 1:
            teacherStudentListMenu(studentList, databaseFile);
            break;
        case 2:
            findStudent(*studentList, databaseFile);
            break;
        case 3:
            addStudent(studentList);
            break;
        case 4:
            deleteStudent(studentList);
            break;
        default:
            printf("\nInvalid choice.\n");
        }
    }
}

void teacherAssessmentMenu(vector_t *studentList, char *databaseFile)
{
    int choice;
    while ((choice = scanMenu(printTeacherAssessmentMenu, NULL)) != 5)
    {
        switch (choice)
        {
        case 1:
            teacherAssessmentsListMenu(studentList, databaseFile);
            break;
        case 2:
            addAssessment(studentList);
            break;
        case 3:
            deleteAssessment(studentList);
            break;
        case 4:
            extractAssessmentFile(databaseFile);
            break;
        default:
            printf("\nInvalid choice.\n");
        }
    }
}

void printTeacherAssessmentMenu(const void *a)
{
    printf("\n\nStudent Main Menu\n");
    printf("1. View all assessments\n"
           "2. Add assessment\n"
           "3. Delete assessment\n"
           "4. Extract assessment file\n"
           "5. Return back to Main Menu\n");
}

void teacherAssessmentsListMenu(vector_t *studentList, char *databaseFile)
{
    vector_t assessments;
    getAllAssessments(*studentList, &assessments);
    int choice;
    while ((choice = scanMenu(printTeacherAssessmentListMenu, &assessments)) != 7)
    {
        switch (choice)
        {
        case 2:
            addAssessment(studentList);
            freeVector(assessments);
            getAllAssessments(*studentList, &assessments);
            break;
        case 3:
            deleteAssessment(studentList);
            freeVector(assessments);
            getAllAssessments(*studentList, &assessments);
            break;
        case 4:
            extractAssessmentFile(databaseFile);
            break;
        case 5:
            sortAllAssessmentList(&assessments);
            break;
        case 6:
            filterAllAssessmentList(*studentList, &assessments);
            break;
        default:
            printf("\nInvalid choice.\n");
        }
    }
    freeVector(assessments);
}

void printTeacherAssessmentListMenu(const void *studentList)
{
    printf("\n\nStudent Main Menu\n");
    displayAllAssessments((vector_t *)studentList);
    printf("1. Close assessment list\n"
           "2. Add assessment\n"
           "3. Delete assessment\n"
           "4. Extract assessment file\n"
           "5. Sort by mark\n"
           "6. Filter by subject\n"
           "7. Return back to Main Menu\n");
}

void displayAllAssessments(vector_t *assessments)
{
    int size = assessments->size;
    printf("-------------------------------------------------");
    printf("------------------------------------------------\n");
    printf("|%s|%s|%s|%s|%s|%s|\n",
           "Student ID     ",
           "First Name     ",
           "Last Name      ",
           "Filename       ",
           "Subject        ",
           "Mark           ");
    printf("-------------------------------------------------");
    printf("------------------------------------------------\n");
    if (size < 1)
    {
        printf("|%-95s|\n",
               " No assesments. Please add some under a student");
    }
    while (size--)
    {
        assessment_student_t *assessment = vectorGet(*assessments, size);
        printf("|%-15d", assessment->studentp->studentId);
        printf("|%-15s", assessment->studentp->firstName);
        printf("|%-15s", assessment->studentp->lastName);
        printf("|%-15.15s", assessment->assessmentp->filename);
        printf("|%-15s", assessment->assessmentp->subject);
        printf("|%15d|\n", assessment->assessmentp->mark);
    }
    printf("-------------------------------------------------");
    printf("------------------------------------------------\n");
}

void addAssessment(vector_t *studentList)
{
    int index = scanFindStudent(*studentList);
    if (index == -1)
        return;

    student_t *student = vectorGet(*studentList, index);
    addStudentAssessments(student);
}

void deleteAssessment(vector_t *studentList)
{
    int index = scanFindStudent(*studentList);
    if (index == -1)
        return;

    student_t *student = vectorGet(*studentList, index);
    deleteStudentAssessment(student);
}

void extractAssessmentFile(char *databaseFile)
{
    if (databaseFile[0] == 0)
    {
        printf("\nNo database has been loaded.\n"
               "Please ensure you have loaded a database.\n"
               "If you added assessment files, you must first save\n"
               "it to a database before it can be extracted.\n");
        printf("You will be returned to the menu\n");
        waitForEnter();
        return;
    }

    char filename[MAX_FILENAME_SIZE];
    if (scanFilename(filename, "Enter the name of the file you wish to extract>"))
        return;

    if (!checkIfFileExistsInDatabase(databaseFile, filename))
    {
        printf("\n%s cannot be found in the database.\n"
               "If you have recently added the assessment file, you must first save\n"
               "it to a database before it can be extracted.\n",
               filename);
        printf("You will be returned to the menu\n");
        waitForEnter();
        return;
    }

    if (scanAreYouSure("Any files in the current directory with same name\n"
                       "will be overwritten.\n"
                       "Are you sure you wish to continue (Y/N)?>"))
        return;

    if (unpackageDatabaseFiles(databaseFile, filename))
    {
        printf("\n\nCannot read database file %s.\n"
               "You will be returned to the menu\n",
               databaseFile);
        waitForEnter();
        return;
    }
}

void sortAllAssessmentList(vector_t *assessments)
{
    printf("1. Ascending\n"
           "2. Descending\n");
    printf("Sort mark by>");
    int option;
    scanf("%d", &option);
    /* Consume newline */
    getchar();
    if (option == 1)
    {
        sort(*assessments, compareAssessmentStudentAsc);
    }
    else if (option == 2)
    {
        sort(*assessments, compareAssessmentStudentDesc);
    }
    else
    {
        printf("Invalid.\n");
        printf("You will be returned to the menu list\n");
        waitForEnter();
    }
}

void filterAllAssessmentList(const vector_t studentList, vector_t *assessments)
{
    freeVector(*assessments);

    char subject[MAX_FILENAME_SIZE];
    printf("Please enter blank to remove filter.\n");
    printf("Enter subject name to filter by>");
    readStringFixLength(subject, MAX_NAME_SIZE);

    if (strcmp(subject, ""))
        getAllAssessmentsFilter(studentList, assessments, subject);
    else
        getAllAssessmentsFilter(studentList, assessments, NULL);
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
            bitFlag = bitFlag ^ (char)pow(2, option - 1);
        else if (option == 5)
            return bitFlag;
        else
            printf("Invalid option.");
    }
}

void saveDatabase(const vector_t studentList, char *databaseFile)
{
    vector_t existingFiles;
    initVector(&existingFiles, sizeof(file_t));
    if (databaseFile[0] != 0)
    {
        readDatabaseToMemory(databaseFile, &existingFiles);
    }

    if (scanFilename(databaseFile, "Enter the database file name>"))
        return;

    char bitFlag = setBitFlag(databaseFile);

    if (writeDatabase(studentList, databaseFile, bitFlag, existingFiles))
    {
        printf("\n\nCannot create database file %s.\n"
               "You will be returned to the menu\n",
               databaseFile);
        waitForEnter();
        return;
    }
    printf("\n\nDatabase successfully saved to %s\n"
           "You will be returned to the menu\n",
           databaseFile);
    waitForEnter();
}

void loadDatabase(vector_t *studentList, char *databaseFile)
{
    scanFilename(databaseFile, "Enter the database file name>");
    if (readDatabase(databaseFile, studentList))
    {
        printf("\n\nCannot read database file %s.\n"
               "You will be returned to the menu.\n",
               databaseFile);
        waitForEnter();
        return;
    }

    printf("\n\nDatabase %s successfully loaded.\n"
           "You will be returned to the menu.\n",
           databaseFile);
    waitForEnter();
}

void extractAll(char *databaseFile)
{
    if (databaseFile[0] == 0)
    {
        printf("\nNo database has been loaded.\n"
               "Please ensure you have loaded a database.\n"
               "If you added assessment files, you must first save\n"
               "it to a database before it can be extracted.\n");
        printf("You will be returned to the menu\n");
        waitForEnter();
        return;
    }

    if (scanAreYouSure("All files in the current directory with same name\n"
                       "as those in the database will be overwritten.\n"
                       "Are you sure you wish to continue (Y/N)?>"))
        return;

    if (unpackageDatabaseFiles(databaseFile, NULL))
    {
        printf("\n\nCannot read database file %s.\n"
               "You will be returned to the menu\n",
               databaseFile);
        waitForEnter();
        return;
    }
}

void printTeacherStudentMainMenu(const void *a)
{
    printf("\n\nStudent Main Menu\n");
    printf("1. View all students\n"
           "2. Search student\n"
           "3. Add student\n"
           "4. Delete student\n"
           "5. Return back to Main Menu\n");
}

void teacherStudentListMenu(vector_t *studentList, char *databaseFile)
{
    int choice;
    while ((choice = scanMenu(printTeacherStudentListMenu, studentList)) != 6 && choice != 1)
    {
        switch (choice)
        {
        case 2:
            findStudent(*studentList, databaseFile);
            break;
        case 3:
            addStudent(studentList);
            break;
        case 4:
            deleteStudent(studentList);
            break;
        case 5:
            sortStudents(studentList);
            break;
        default:
            printf("\nInvalid choice.\n");
        }
    }
}

void printTeacherStudentListMenu(const void *studentList)
{
    printf("\n\nStudent List Menu\n");
    displayAllStudents((vector_t *)studentList);
    printf("1. Close student list\n"
           "2. Search student\n"
           "3. Add student\n"
           "4. Delete student\n"
           "5. Sort students\n"
           "6. Return back to Main Menu\n");
}

void displayAllStudents(vector_t *studentList)
{
    int size = studentList->size;
    printf("-------------------------------------------------\n");
    printf("|%s|%s|%s|\n",
           "Student ID     ",
           "First Name     ",
           "Last Name      ");
    printf("-------------------------------------------------\n");
    if (size < 1)
    {
        printf("|%-47s|\n", " No students, please add some.");
    }
    while (size--)
    {
        student_t *student = vectorGet(*studentList, size);
        printf("|%-15d", student->studentId);
        printf("|%-15s", student->firstName);
        printf("|%-15s|\n", student->lastName);
    }
    printf("-------------------------------------------------\n");
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

void findStudent(const vector_t studentList, char *databaseFile)
{
    int index = scanFindStudent(studentList);
    if (index == -1)
        return;

    student_t *maybeStudent = vectorGet(studentList, index);
    teacherStudentMenu(maybeStudent, databaseFile);
}

void addStudent(vector_t *studentList)
{
    student_t newStudent;
    initStudent(&newStudent);
    printf("Enter student ID>");
    scanf("%d", &newStudent.studentId);
    /* Consume trailing newline */
    if (flushScan() > 0)
    {
        printf("Invalid.\n");
        printf("You will be returned to the menu list\n");
        waitForEnter();
        return;
    }

    if (searchStudentIndex(*studentList, newStudent.studentId) != -1)
    {
        printf("This student ID %d already exists in the database.\n",
               newStudent.studentId);
        printf("You will be returned to the menu list\n");
        waitForEnter();
        return;
    }

    printf("Enter first name>");
    readStringFixLength(newStudent.firstName, MAX_NAME_SIZE);

    printf("Enter last name>");
    readStringFixLength(newStudent.lastName, MAX_NAME_SIZE);

    vectorPushBack(studentList, &newStudent);
}

void deleteStudent(vector_t *studentList)
{
    int index = scanFindStudent(*studentList);
    if (index == -1)
        return;

    student_t *student = vectorGet(*studentList, index);
    int studentId = student->studentId;
    vectorRemove(studentList, index);
    printf("%d has been deleted.\n", studentId);
    waitForEnter();
}

void sortStudents(vector_t *studentList)
{
    printf("1. Ascending\n"
           "2. Descending\n");
    printf("Sort student ID by>");
    int option;
    scanf("%d", &option);
    /* Consume newline */
    getchar();
    if (option == 1)
    {
        sort(*studentList, compareStudentAsc);
    }
    else if (option == 2)
    {
        sort(*studentList, compareStudentDesc);
    }
    else
    {
        printf("Invalid.\n");
        printf("You will be returned to the menu list\n");
        waitForEnter();
    }
}

void teacherStudentMenu(student_t *student, char *databaseFile)
{
    int choice;
    while ((choice = scanMenu(printTeacherStudentMenu, student)) != 5)
    {
        switch (choice)
        {
        case 1:
            studentAssessmentList(student, databaseFile);
            break;
        case 2:
            addStudentAssessments(student);
            break;
        case 3:
            deleteStudentAssessment(student);
            break;
        case 4:
            extractStudentAssessmentFile(student, databaseFile);
            break;
        default:
            printf("\nInvalid choice.\n");
        }
    }
}

void printTeacherStudentMenu(const void *a)
{
    const student_t *student = a;
    printf("\n\nStudent Menu\n");
    printf("For student Id: %d\n", student->studentId);
    printf("Name: %s %s\n", student->lastName, student->firstName);
    printf("1. View students assessments\n"
           "2. Add assessment\n"
           "3. Delete assessment\n"
           "4. Extract Assessment File\n"
           "5. Return back to Student Main Menu\n");
}

void studentAssessmentList(student_t *student, char *databaseFile)
{
    int choice;
    while ((choice = scanMenu(printStudentAssessmentList, student)) != 6 && choice != 1)
    {
        switch (choice)
        {
        case 2:
            addStudentAssessments(student);
            break;
        case 3:
            deleteStudentAssessment(student);
            break;
        case 4:
            extractStudentAssessmentFile(student, databaseFile);
            break;
        default:
            printf("\nInvalid choice.\n");
        }
    }
}

void printStudentAssessmentList(const void *a)
{
    const student_t *student = a;
    printf("\n\nStudent Menu\n");
    printf("For student Id: %d\n", student->studentId);
    printf("Name: %s %s\n", student->lastName, student->firstName);
    displayStudentAssessments(*student);
    printf("1. Close assessment list\n"
           "2. Add assessment\n"
           "3. Delete assessment\n"
           "4. Extract Assessment File\n"
           "5. Sort by Mark\n"
           "6. Return back to Student Main Menu\n");
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

void addStudentAssessments(student_t *student)
{
    assessment_t newAssessment;

    printf("Enter subject name>");
    readStringFixLength(newAssessment.subject, MAX_NAME_SIZE);

    if (scanFilename(newAssessment.filename, "Enter the assessment file name>"))
        return;

    if (!checkIfFileExists(newAssessment.filename))
    {
        printf("\n\nCannot find file %s.\n"
               "You will be returned to the menu\n",
               newAssessment.filename);
        waitForEnter();
        return;
    }

    while (1)
    {
        printf("Enter the mark>");
        scanf("%d", &newAssessment.mark);
        /* Consume trailing newline */
        if (flushScan() != 0 ||
            newAssessment.mark < 0 || newAssessment.mark > 100)
        {
            printf("\nInvalid\n");
            printf("Please enter a mark between 0 and 100\n");
        }
        else
        {
            break;
        }
    }

    vectorPushBack(&student->assessments, &newAssessment);
}

void deleteStudentAssessment(student_t *student)
{
    char filename[MAX_FILENAME_SIZE];
    if (scanFilename(filename, "Enter the assessment file name>"))
        return;
    int index = searchStudentAssessmentIndex(*student, filename);
    if (index == -1)
    {
        printf("\n\nNo assessment found with filename %s\n", filename);
        printf("You will you be returned to the menu.\n");
        waitForEnter();
    }
    vectorRemove(&student->assessments, index);
}

void extractStudentAssessmentFile(student_t *student, char *databaseFile)
{
    /* TODO: Implement student assessment file*/
    extractAssessmentFile(databaseFile);
}