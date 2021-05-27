#include <string.h> /* strcpy */
#include <stdio.h>  /* printf, scanf */
#include <math.h>   /* pow */
#include "interface.h"
#include "teacher_interface.h"
#include "database.h"
#include "vector.h"
#include "student.h"
#include "sort.h"

/*****************************************************************************
 * Private function prototypes
*****************************************************************************/
void teacherMainMenu(void);
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
void filterAllAssessmentList(const vector_t studentList,
                             vector_t *assessments);
void printTeacherAssessmentListMenu(const void *a);

void teacherStudentMenu(student_t *student, char *databaseFile);
void printTeacherStudentMenu(const void *a);
void studentAssessmentList(student_t *student, char *databaseFile);
void addStudentAssessments(student_t *student);
void deleteStudentAssessment(student_t *student);
void extractStudentAssessmentFile(student_t *student, char *databaseFile);

void printStudentAssessmentList(const void *a);

/*****************************************************************************
 * This function is the main entry point to the teacher menu and 
 * Handles the interface for Teacher Main Menu. 
 * It calls the function to display the Student Main Menu and handle the 
 * input choice.
 * Input:
 *   None
 * Post:
 *   Prints the menu to stdout and redirects to choice selected.
*****************************************************************************/
void teacherMainMenu(void)
{
    /* Initiate studentList and databaseFile variables */
    vector_t studentList;
    initVector(&studentList, sizeof(student_t));
    char databaseFile[MAX_FILENAME_SIZE];
    databaseFile[0] = 0;

    /* Print the welcome message */
    printLaunch();

    /* Loop over this menu until 6 is entered.
       When the user selects another menu they 
       will be redirected here on return back. */
    int choice;
    while ((choice = scanMenu(printTeacherMainMenu, NULL)) != 6)
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

/*****************************************************************************
 * This function prints the welcome message.
 * Input:
 *   None
 * Post:
 *   Launch message is printed to stdout.
*****************************************************************************/
void printLaunch(void)
{
    printf("\n\n"
           "Welcome to student assessment file management\n");
}

/*****************************************************************************
 * This function prints the Teacher Main Menu Text
 * Input:
 *   arg - Unused parameter. Only listed to satisfy scanMenu()
 * Post:
 *   Teacher Main Menu text is printed to stdout
*****************************************************************************/
void printTeacherMainMenu(const void *arg)
{
    printf("\n\nMain Menu\n");
    printf("1. Student menu\n"
           "2. Assessment menu\n"
           "3. Save to database\n"
           "4. Load database\n"
           "5. Extract all files\n"
           "6. Exit the program\n");
}

/*****************************************************************************
 * This function handles the interface for Teacher Student Main Menu. 
 * It calls the function to display the Teacher Student Main Menu and 
 * handles the input choice.
 * Input:
 *   studentList - The list of students.
 *   databaseFile - The database file name.
 * Post:
 *   Prints the menu to stdout and redirects to choice selected.
*****************************************************************************/
void teacherStudentMainMenu(vector_t *studentList, char *databaseFile)
{
    /* Loop over this menu until 5 is entered.
       When the user selects another menu they 
       will be redirected here on return back. */
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

/*****************************************************************************
 * This function handles the interface for Teacher Assessment Main Menu. 
 * It calls the function to display the Teacher Assessment Main Menu and 
 * handles the input choice.
 * Input:
 *   studentList - The list of students.
 *   databaseFile - The database file name.
 * Post:
 *   Prints the menu to stdout and redirects to choice selected.
*****************************************************************************/
void teacherAssessmentMenu(vector_t *studentList, char *databaseFile)
{
    /* Loop over this menu until 5 is entered.
       When the user selects another menu they 
       will be redirected here on return back. */
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

/*****************************************************************************
 * This function prints the Teacher Assessment Menu Text
 * Input:
 *   arg - Unused parameter. Only listed to satisfy scanMenu()
 * Post:
 *   Teacher Assessment Menu text is printed to stdout
*****************************************************************************/
void printTeacherAssessmentMenu(const void *arg)
{
    printf("\n\nStudent Main Menu\n");
    printf("1. Open all assessments list\n"
           "2. Add assessment\n"
           "3. Delete assessment\n"
           "4. Extract assessment file\n"
           "5. Return back to Main Menu\n");
}

/*****************************************************************************
 * This function handles the interface for Teacher Assessment List Menu. 
 * It calls the function to display the Teacher Assessment List Menu and 
 * handles the input choice.
 * Input:
 *   studentList - The list of students.
 *   databaseFile - The database file name.
 * Post:
 *   Prints the menu to stdout and redirects to choice selected.
*****************************************************************************/
void teacherAssessmentsListMenu(vector_t *studentList, char *databaseFile)
{
    /* Convert student assessments into a single vector. */
    vector_t assessments;
    getAllAssessments(*studentList, &assessments);
    /* Loop over this menu until 7 or 1 is entered.
       When the user selects another menu they 
       will be redirected here on return back. */
    int choice;
    while ((choice = scanMenu(printTeacherAssessmentListMenu,
                              &assessments)) != 7 &&
           choice != 1)
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

/*****************************************************************************
 * This function prints the Teacher Assessment List Text, including the
 * assessments
 * Input:
 *   studentList - a void pointer to a studentList of vector_t. 
 *                 Must be this type to satisfy scanMenu()
 * Post:
 *   Teacher Assessment List text with the assessments is printed to stdout
*****************************************************************************/
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
           "7. Return back\n");
}

/*****************************************************************************
 * This function prints the table off all assessments.
 * Along with the student.
 * Input:
 *   assessments - The vector of assessments to be printed.
 * Post:
 *   A table of assessments with the student is printed to stdout.
*****************************************************************************/
void displayAllAssessments(vector_t *assessments)
{
    /* Print the table header. */
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
    /* If there are no assessments. Print message. */
    int size = assessments->size;
    if (size < 1)
    {
        printf("|%-95s|\n",
               " No assesments. Please add some under a student");
    }
    /* Loop over every assessment. */
    while (size--)
    {
        /* Print the assessment with the student. */
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

/*****************************************************************************
 * This function scans the user for a student ID and then prompts them for
 * the assessment details to add under that student.
 * Input:
 *   studentList - The vector of all students.
 * Post:
 *   If a student is found, and valid assessment details are entered.
 *   The student will now have an assessment added to them.
*****************************************************************************/
void addAssessment(vector_t *studentList)
{
    /* scan the user for the student id.
       If the student cannot before return back to the menu. */
    int index = scanFindStudent(*studentList);
    if (index == -1)
        return;

    /* If found, prompt the user to enter the assessment details.
       to be added to the found student.  */
    student_t *student = vectorGet(*studentList, index);
    addStudentAssessments(student);
}

/*****************************************************************************
 * This function scans the user for a student ID and assessment file name.
 * If found it will delete that assessment.
 * Input:
 *   studentList - The vector of all students.
 * Post:
 *   If a student is found, and the filename was found. The assessment will
 *   be deleted.
*****************************************************************************/
void deleteAssessment(vector_t *studentList)
{
    /* scan the user for the student id.
       If the student cannot before return back to the menu. */
    int index = scanFindStudent(*studentList);
    if (index == -1)
        return;

    /* If found, 
       now prompt the user to enter the assessment filename for deletion. */
    student_t *student = vectorGet(*studentList, index);
    deleteStudentAssessment(student);
}

/*****************************************************************************
 * This function scans the user for an assessment filename and if found,
 * the file will be extracted to the current directory.
 * Input:
 *   databaseFile - The name of the database to look for the file in.
 * Post:
 *   If found, the assessment file will be extracted to the 
 *   current directory.
*****************************************************************************/
void extractAssessmentFile(char *databaseFile)
{
    /* Check that the databaseFile is not the initialised value.
       If it is, it means the user is yet to load a database.
       Inform them to load a database. */
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

    /* Scan the user for the filename to extract. */
    char filename[MAX_FILENAME_SIZE];
    if (scanFilename(filename,
                     "Enter the name of the file you wish to extract>"))
        return;

    /* Check the entered filename exists in the loaded database. */
    if (!checkIfFileExistsInDatabase(databaseFile, filename))
    {
        printf("\n%s cannot be found in the database.\n"
               "If you have recently added the assessment file,"
               " you must first save\n"
               "it to a database before it can be extracted.\n",
               filename);
        printf("You will be returned to the menu\n");
        waitForEnter();
        return;
    }

    /* Give the user a final warning that 
       a file with the same name will be overwritten. */
    if (scanAreYouSure("Any files in the current directory with same name\n"
                       "will be overwritten.\n"
                       "Are you sure you wish to continue (Y/N)?>"))
        return;

    /* Perform the extraction of the file. */
    if (unpackageDatabaseFiles(databaseFile, filename))
    {
        printf("\n\nCannot read database file %s.\n"
               "You will be returned to the menu\n",
               databaseFile);
        waitForEnter();
        return;
    }
}

/*****************************************************************************
 * This function scans the user for how they would like to sort the all
 * assessments list. And performs a sort in that way.
 * Input:
 *   assessments - The list of assessments to be sorted.
 * Post:
 *   assesments will now be sorted by either ascending 
 *   or descending order depending on option selected.
*****************************************************************************/
void sortAllAssessmentList(vector_t *assessments)
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

/*****************************************************************************
 * This function filters a all assessments by subject name.
 * Input:
 *   studentList - The list of students containing all assessments.
 *   assessments - List of assesments to display the table of assesments.
 * Post:
 *   assessments will contain the filtered assessments.
*****************************************************************************/
void filterAllAssessmentList(const vector_t studentList,
                             vector_t *assessments)
{
    /* Scan the subject to filter by. */
    freeVector(*assessments);
    char subject[MAX_FILENAME_SIZE];
    printf("Please enter blank to remove filter.\n");
    printf("Enter subject name to filter by>");
    readStringFixLength(subject, MAX_NAME_SIZE);

    /* If the user has entered blank
       add all assessments to the student to be displayed.
       Otherwise apply the filter. */
    if (strcmp(subject, ""))
        getAllAssessmentsFilter(studentList, assessments, subject);
    else
        getAllAssessmentsFilter(studentList, assessments, NULL);
}

/*****************************************************************************
 * This function scans the user for the database saving options.
 * i.e. the bit flag.
 * Input:
 *   filename - The filename to save the new database to. 
 *              Used to display to the user.
 * Return:
 *   the bitFlag configured by the user.
*****************************************************************************/
char setBitFlag(char *filename)
{
    /* Loop until user enters 5. */
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
        /* Scan the users choice. */
        printf("Enter your choice>");
        int option;
        scanf("%d", &option);
        /* Consume trailing newline */
        flushScan();
        /* If they enter between 0 and 4, 
           apply that value to the bit flag.
           If 5 is entered, return. */
        if (0 < option && option <= 4)
            bitFlag = bitFlag ^ (char)pow(2, option - 1);
        else if (option == 5)
            return bitFlag;
        else
            printf("Invalid option.");
    }
}

/*****************************************************************************
 * This function saves a database to a file. i.e. saves a studentList to a
 * file.
 * Input:
 *   studentList - The list of students to save to the database.
 *   databaseFile - The filename to save the new database to.
 * Post:
 *   If successful, a file named databaseFile will be created with the
 *   database inside. 
*****************************************************************************/
void saveDatabase(const vector_t studentList, char *databaseFile)
{
#ifdef DEBUG
    printf("Existing Database: %s\n", databaseFile);
#endif
    /* Get all the existing files from the current database. */
    vector_t existingFiles;
    initVector(&existingFiles, sizeof(file_t));
    if (databaseFile[0] != 0)
    {
        /* Only if the databaseFile is not the initialised value. */
        if (readDatabaseToMemory(databaseFile, &existingFiles))
        {
            printf("Failed to read existing database\n");
            return;
        }
    }

#ifdef DEBUG
    printf("Existing file count: %d\n", existingFiles.size);
#endif

    /* Scan the new database filename. */
    if (scanFilename(databaseFile, "Enter the database file name>"))
        return;

    /* Get the user to configure the bitFlag. */
    char bitFlag = setBitFlag(databaseFile);

    /* Write the new database with the enter configuration. */
    if (writeDatabase(studentList, databaseFile, bitFlag, existingFiles))
    {
        printf("\n\nCannot create database file %s.\n"
               "You will be returned to the menu\n",
               databaseFile);
        waitForEnter();
        return;
    }

    /* If all was successful inform the user,
       Otherwise, user will have been informed on failure and returned. */
    printf("\n\nDatabase successfully saved to %s\n"
           "You will be returned to the menu\n",
           databaseFile);
    waitForEnter();
}

/*****************************************************************************
 * This function loads a database from a file. i.e. loads studentList from a
 * file.
 * Input:
 *   studentList - The list of students to load the students read into.
 *   databaseFile - The filename to load the database from.
 * Post:
 *   If successful, studentList will contain the students from the database. 
*****************************************************************************/
void loadDatabase(vector_t *studentList, char *databaseFile)
{
    /* Scan the database filename. */
    scanFilename(databaseFile, "Enter the database file name>");

    /* Read the database into studentList. */
    if (readDatabase(databaseFile, studentList))
    {
        printf("\n\nCannot read database file %s.\n"
               "You will be returned to the menu.\n",
               databaseFile);
        waitForEnter();
        return;
    }

    /* If all was successful inform the user,
       Otherwise, user will have been informed on failure and returned. */
    printf("\n\nDatabase %s successfully loaded.\n"
           "You will be returned to the menu.\n",
           databaseFile);
    waitForEnter();
}

/*****************************************************************************
 * This function extracts all assessment files from a database.
 * Input:
 *   databaseFile - The name of the database to extract files from.
 * Post:
 *   If found, all assessment files will be extracted to the 
 *   current directory. 
*****************************************************************************/
void extractAll(char *databaseFile)
{
    /* Check that the databaseFile is not the initialised value.
       If it is, it means the user is yet to load a database.
       Inform them to load a database. */
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

    /* Give the user a final warning that 
       a file with the same name will be overwritten. */
    if (scanAreYouSure("All files in the current directory with same name\n"
                       "as those in the database will be overwritten.\n"
                       "Are you sure you wish to continue (Y/N)?>"))
        return;

    /* Perform the extraction of the file. */
    if (unpackageDatabaseFiles(databaseFile, NULL))
    {
        printf("\n\nCannot read database file %s.\n"
               "You will be returned to the menu\n",
               databaseFile);
        waitForEnter();
        return;
    }
}

/*****************************************************************************
 * This function prints the Teacher Student Main Menu Text
 * Input:
 *   arg - Unused parameter. Only listed to satisfy scanMenu()
 * Post:
 *   Teacher Student Main Menu text is printed to stdout
*****************************************************************************/
void printTeacherStudentMainMenu(const void *arg)
{
    printf("\n\nStudent Main Menu\n");
    printf("1. Open student list\n"
           "2. Search student\n"
           "3. Add student\n"
           "4. Delete student\n"
           "5. Return back to Main Menu\n");
}

/*****************************************************************************
 * This function handles the interface for Teacher Student List Menu. 
 * It calls the function to display the Teacher Student List Menu and 
 * handles the input choice.
 * Input:
 *   studentList - The list of students.
 *   databaseFile - The database file name.
 * Post:
 *   Prints the menu to stdout and redirects to choice selected.
*****************************************************************************/
void teacherStudentListMenu(vector_t *studentList, char *databaseFile)
{
    /* Loop over this menu until 6 or 1 is entered.
       When the user selects another menu they 
       will be redirected here on return back. */
    int choice;
    while ((choice = scanMenu(printTeacherStudentListMenu,
                              studentList)) != 6 &&
           choice != 1)
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

/*****************************************************************************
 * This function prints the Teacher Student List Text
 * Input:
 *   studentList - a void pointer to a studentList of vector_t. 
 *                 Must be this type to satisfy scanMenu()
 * Post:
 *   Teacher Student List text with the students is printed to stdout
*****************************************************************************/
void printTeacherStudentListMenu(const void *studentList)
{
    printf("\n\nStudent List Menu\n");
    displayAllStudents((vector_t *)studentList);
    printf("1. Close student list\n"
           "2. Search student\n"
           "3. Add student\n"
           "4. Delete student\n"
           "5. Sort students\n"
           "6. Return back\n");
}

/*****************************************************************************
 * This function prints the table off all students.
 * Input:
 *   studentList - The vector of students to be printed in a table.
 * Post:
 *   A table of all students is printed to stdout.
*****************************************************************************/
void displayAllStudents(vector_t *studentList)
{
    /* Print the table header. */
    printf("-------------------------------------------------\n");
    printf("|%s|%s|%s|\n",
           "Student ID     ",
           "First Name     ",
           "Last Name      ");
    printf("-------------------------------------------------\n");
    /* If there are no assessments. Print message. */
    int size = studentList->size;
    if (size < 1)
    {
        printf("|%-47s|\n", " No students, please add some.");
    }
    /* Loop over every assessment. */
    while (size--)
    {
        /* Print the assessment with the student. */
        student_t *student = vectorGet(*studentList, size);
        printf("|%-15d", student->studentId);
        printf("|%-15s", student->firstName);
        printf("|%-15s|\n", student->lastName);
    }
    printf("-------------------------------------------------\n");
}

/*****************************************************************************
 * This function scans for a student ID and attempts to find it in the
 * student list.
 * Input:
 *   studentList - The vector of students to be searched.
 * Return:
 *   The position of the student in studentList
 *   -1 if the student ID cannot be found.
*****************************************************************************/
int scanFindStudent(const vector_t studentList)
{
    /* scan the user for the student id. */
    int studentId;
    printf("Enter the ID of the student>");
    scanf("%d", &studentId);
    /* Consume trailing newline */
    flushScan();

    /* Search for that student ID in the list of students */
    int index = searchStudentIndex(studentList, studentId);
    if (index == -1)
    {
        printf("\n\nNo student found with ID %d\n", studentId);
        printf("You will you be returned to the menu.\n");
        waitForEnter();
    }

    /* Return the index found. */
    return index;
}

/*****************************************************************************
 * This function handles the user querying of a student. The user is prompted
 * for a student ID and then redirected to a the student menu with that
 * student populated.
 * Input:
 *   studentList - The vector of students to be searched.
 *   databaseFile - The database file name.
 * Post:
 *   If the student ID is found, the user is directed to the Student Menu
 *   with the found students information.
*****************************************************************************/
void findStudent(const vector_t studentList, char *databaseFile)
{
    /* scan the user for the student id.
       If the student cannot before return back to the menu. */
    int index = scanFindStudent(studentList);
    if (index == -1)
        return;

    /* If found, direct the user to the Student Menu
       for the found student. */
    student_t *maybeStudent = vectorGet(studentList, index);
    teacherStudentMenu(maybeStudent, databaseFile);
}

/*****************************************************************************
 * This function creates a new student by scanning the user for the students
 * information.
 * Input:
 *   studentList - The vector of all students.
 * Post:
 *   If successful, studentList will have the new created student
 *   appended to it.
*****************************************************************************/
void addStudent(vector_t *studentList)
{
    /* Initialise the new student. */
    student_t newStudent;
    initStudent(&newStudent);

    /* Scan the students ID. */
    printf("Enter student ID>");
    scanf("%d", &newStudent.studentId);
    /* Consume trailing newline and check that the input was valid. */
    if (flushScan() > 0)
    {
        /* If the input was not valid 
           inform the user and return back to the menu. */
        printf("Invalid.\n");
        printf("You will be returned to the menu list\n");
        waitForEnter();
        return;
    }

    /* Check that the student ID does not already exist. */
    if (searchStudentIndex(*studentList, newStudent.studentId) != -1)
    {
        printf("This student ID %d already exists in the database.\n",
               newStudent.studentId);
        printf("You will be returned to the menu list\n");
        waitForEnter();
        return;
    }

    /* Scan the first name */
    printf("Enter first name>");
    readStringFixLength(newStudent.firstName, MAX_NAME_SIZE);

    /* Scan the last name */
    printf("Enter last name>");
    readStringFixLength(newStudent.lastName, MAX_NAME_SIZE);

    /* Push the new student onto the student list vector. */
    vectorPushBack(studentList, &newStudent);
}

/*****************************************************************************
 * This function deletes a student by scanning the user for the student ID
 * of the user to delete.
 * Input:
 *   studentList - The vector of all students.
 * Post:
 *   If successful, studentList will no longer contain the student.
*****************************************************************************/
void deleteStudent(vector_t *studentList)
{
    /* scan the user for the student id.
       If the student cannot before return back to the menu. */
    int index = scanFindStudent(*studentList);
    if (index == -1)
        return;

    /* Get the student from the list. */
    student_t *student = vectorGet(*studentList, index);
    /* Then get its student ID to inform the user it has been deleted.
       And remove it from the vector. */
    int studentId = student->studentId;
    vectorRemove(studentList, index);
    printf("%d has been deleted.\n", studentId);
    waitForEnter();
}

/*****************************************************************************
 * This function scans the user for how they would like to sort the student
 * list. And performs a sort in that way.
 * Input:
 *   studentList - The list of students whos are to be sorted.
 * Post:
 *   studentList will now be sorted by either ascending 
 *   or descending order depending on option selected.
*****************************************************************************/
void sortStudents(vector_t *studentList)
{
    /* Ask the user if they want to sort ascending or descending. */
    printf("1. Ascending\n"
           "2. Descending\n");
    printf("Sort student ID by>");
    int option;
    scanf("%d", &option);
    /* Consume newline */
    flushScan();

    /* Apply the option they have picked.
       If neither, return to menu. */
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

/*****************************************************************************
 * This function handles the interface for Teacher Student Menu. 
 * It calls the function to display the Teacher Student Menu and 
 * handles the input choice.
 * Input:
 *   student - The student who menu is being displayed.
 *   databaseFile - The database file name.
 * Post:
 *   Prints the menu to stdout and redirects to choice selected.
*****************************************************************************/
void teacherStudentMenu(student_t *student, char *databaseFile)
{
    /* Loop over this menu until 5 is entered.
       When the user selects another menu they 
       will be redirected here on return back. */
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

/*****************************************************************************
 * This function prints the Teacher Student List Menu, including the
 * student info.
 * Input:
 *   student - a void pointer to a student of student_t. 
 *             Must be this type to satisfy scanMenu()
 * Post:
 *   Teacher Student Menu text with the student info is printed to stdout
*****************************************************************************/
void printTeacherStudentMenu(const void *arg)
{
    /* Cast the arg to its type of student_t. */
    const student_t *student = arg;
    printf("\n\nStudent Menu\n");
    printf("For...\nStudent ID: %d\n", student->studentId);
    printf("Name: %s %s\n", student->lastName, student->firstName);
    printf("1. Open students assessment list\n"
           "2. Add assessment\n"
           "3. Delete assessment\n"
           "4. Extract Assessment File\n"
           "5. Return back\n");
}

/*****************************************************************************
 * This function handles the interface for Teacher Assessment List for a 
 * student. It calls the function to display the Teacher Assessment 
 * List Menu and handles the input choice.
 * Input:
 *   student - The student whos assessment menu is to be displayed.
 *   databaseFile - The database file name.
 * Post:
 *   Prints the menu to stdout and redirects to choice selected.
*****************************************************************************/
void studentAssessmentList(student_t *student, char *databaseFile)
{
    /* Loop over this menu until 7 or 1 is entered.
       When the user selects another menu they 
       will be redirected here on return back. */
    int choice;
    while ((choice = scanMenu(printStudentAssessmentList, student)) != 6 &&
           choice != 1)
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
        case 5:
            sortStudentAssessmentFile(student);
            break;
        default:
            printf("\nInvalid choice.\n");
        }
    }
}

/*****************************************************************************
 * This function prints the Teacher Student Assessment List Text, 
 * for the a particular student.
 * Input:
 *   arg - a void pointer to a student of student_t. 
 *         Must be this type to satisfy scanMenu()
 * Post:
 *   Assessment List for a student is printed to stdout
*****************************************************************************/
void printStudentAssessmentList(const void *arg)
{
    /* Cast the arg to its types of */
    const student_t *student = arg;
    printf("\n\nStudent Menu\n");
    printf("For...\nStudent ID: %d\n", student->studentId);
    printf("Name: %s %s\n", student->lastName, student->firstName);
    displayStudentAssessments(*student);
    printf("1. Close students assessment list\n"
           "2. Add assessment\n"
           "3. Delete assessment\n"
           "4. Extract Assessment File\n"
           "5. Sort by Mark\n"
           "6. Return back\n");
}

/*****************************************************************************
 * This function creates a new assessment under a given student.
 * Input:
 *   student - The student which the assessment is to be created under.
 * Post:
 *   If successful, student will contain the newly created assessment.
*****************************************************************************/
void addStudentAssessments(student_t *student)
{
    assessment_t newAssessment;
    /* Scan for the subject name. */
    printf("Enter subject name>");
    readStringFixLength(newAssessment.subject, MAX_NAME_SIZE);

    /* Scan for the assessment file name */
    if (scanFilename(newAssessment.filename,
                     "Enter the assessment file name>"))
        return;

    /* Check that the assessment file exists */
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
        /* Scan for the assessment mark */
        printf("Enter the mark>");
        scanf("%d", &newAssessment.mark);
        /* Consume trailing newline */

        /* Ensure it is valid and is within 0 and 100 */
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

    /* Push the new assessment onto the students assessments. */
    vectorPushBack(&student->assessments, &newAssessment);
}

/*****************************************************************************
 * This function deletes an assessment under a given student.
 * Input:
 *   student - The student which the assessment is to be deleted from.
 * Post:
 *   If successful, student will no longer contain the assessment.
*****************************************************************************/
void deleteStudentAssessment(student_t *student)
{
    /* Scan for the filename. */
    char filename[MAX_FILENAME_SIZE];
    if (scanFilename(filename, "Enter the assessment file name>"))
        return;

    /* Search for the assessment file on the student. */
    int index = searchStudentAssessmentIndex(*student, filename);
    if (index == -1)
    {
        printf("\n\nNo assessment found with filename %s\n", filename);
        printf("You will you be returned to the menu.\n");
        waitForEnter();
    }

    /* if it exists remove it from the students assessments. */
    vectorRemove(&student->assessments, index);
}

/*****************************************************************************
 * This function extracts an assessment file from the student menu by
 * reusing the function from the assessment menu.
 * Input:
 *   student - The student which the assessment is to be extracted from.
 *   databaseFile - The database filename.
 * Post:
 *   If successful, the assessment file will be extracted to the current
 *   directory.
*****************************************************************************/
void extractStudentAssessmentFile(student_t *student, char *databaseFile)
{
    /* Scan the user for the assessment file name
       and extract it to the current directory. */
    extractAssessmentFile(databaseFile);
}