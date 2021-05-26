/*****************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Database management.
 * Functions for managing the database. This includes reading and writing
 * student information. Packaging one or more files together and compression
 * and/or encrypting the assessment files.
*****************************************************************************/
#include <stdio.h>
#include <string.h> /* strlen */
#include "database.h"
#include "vector.h"
#include "encrypt.h"
#include "compress.h"
#include "secure_hash.h"
#include "student.h"
#include "search.h"

#define TEMP_DATABASE_NAME "database.bin.tmp"

/*****************************************************************************
 * Private function prototypes
*****************************************************************************/
void copyFile(FILE *dest, FILE *src);
void copyHeader(FILE *dest, FILE *src);
void writeAssessment(assessment_t *assessment, FILE *out_fp);
void writeStudentAssessment(student_t *student, FILE *out_fp);
void writeStudent(student_t *student, FILE *out_fp);
void writeFileContents(char *filename, FILE *out_fp);
void writeFileTContents(file_t file, FILE *out_fp);
int compareFilenames(const void *ap, const void *bp);
void writeFiles(FILE *out_fp, vector_t studentList, vector_t existingFiles);
void writeHeader(FILE *out_fp, vector_t studentList);
int XOREncryptDatabase(FILE *database_fp, long startPos);
int shiftEncryptDatabase(FILE *database_fp, long startPos);
int huffmanCompressDatabase(FILE **database_fp, long startPos,
                            char *out_file);
int XORDecryptDatabase(FILE **database_fp, char *outFile);
int shiftDecryptDatabase(FILE **database_fp, char *outFile);
int huffmanDecompressDatabase(FILE **database_fp, char *outFile);
void readAssessment(assessment_t *assessment, FILE *in_fp);
void readStudentAssessment(student_t *student, FILE *in_fp);
void readStudent(student_t *student, FILE *in_fp);
void readHeader(FILE *database_fp, vector_t *studentList);
void readDatabaseFp(FILE *database_fp, vector_t *filenames);
void separateFilesToMemory(FILE *database_fp, vector_t filenames,
                           vector_t *files);
void separateFiles(FILE *database_fp, vector_t filenames);
void separateFilesFilter(FILE *database_fp, vector_t filenames,
                         char *filenameFilter);
int unpackageDatabaseFilesContents(FILE *database_fp, char *files);
int checkIfFileExists(char *filename);

/*****************************************************************************
 * Given a filename, this function will check if the file exists.
 * Input:
 *   filename - The name of the file to check if it exists.
 * Return:
 *   1 - The file does exist.
 *   0 - The file does not exist.
*****************************************************************************/
int checkIfFileExists(char *filename)
{
    FILE *filep;
    if ((filep = fopen(filename, "r")))
    {
        fclose(filep);
        return 1;
    }
    return 0;
}

/*****************************************************************************
 * Given a filename, this function will check if the file exists in the
 * database.
 * Input:
 *   databaseFile - The database file to verify if the file exist.
 *   filename - The name of the file to check if it exists.
 * Return:
 *   1 - The file does exist.
 *   0 - The file does not exist.
*****************************************************************************/
int checkIfFileExistsInDatabase(char *databaseFile, char *filename)
{
    /* Read all students in stored database. */
    vector_t studentList;
    readDatabase(databaseFile, &studentList);

    /* Get all filenames in the database. */
    vector_t filenames;
    getAllFilenames(studentList, &filenames);
    int index = search(filenames, compareString, filename);

    /* Free vectors and return result. */
    freeVector(studentList);
    freeVector(filenames);
    return index != -1;
}

/*****************************************************************************
 * This function copys the bytes of one file pointer to another until
 * end-of-file is reached from the source file.
 * Input:
 *   dest - The destination file pointer to copy the bytes to.
 *   src - The source file pointer of the bytes to be copied from.
 * Post:
 *   dest - Will have all the bytes in src appended to it.
*****************************************************************************/
void copyFile(FILE *dest, FILE *src)
{
    int buffer;
    while ((buffer = getc(src)) != EOF)
    {
        putc(buffer, dest);
    }
}

/*****************************************************************************
 * This function copys the header information of a file only. It seeks the
 * end of the header to get the header length, then copies until that
 * length is exceeded.
 * Input:
 *   dest - The destination file pointer to copy the header to.
 *   src - The source file pointer of the header to be copied from.
 * Post:
 *   dest - Will have the header bytes in src appended to it.
*****************************************************************************/
void copyHeader(FILE *dest, FILE *src)
{
    /* Seek the end of the header */
    vector_t temp;
    initVector(&temp, sizeof(char) * 255);
    readHeader(src, &temp);
    freeVector(temp);
    char bitFlag;
    fread(&bitFlag, sizeof(char), 1, src);

    /* Get where header ends */
    long pos = ftell(src);
    fseek(src, 0, SEEK_SET);

    /* Copy over the header */
    int buffer;
    while ((buffer = getc(src)) != EOF && pos--)
    {
        putc(buffer, dest);
    }
}

/*****************************************************************************
 * This function writes an assessment struct to a file pointer.
 * Input:
 *   assessmentp - Pointer to the assessment to be written
 *   out_fp - File pointer to write the assessment to.
 * Post:
 *   out_fp - Will have assessment written to it.
*****************************************************************************/
void writeAssessment(assessment_t *assessmentp, FILE *out_fp)
{
    /* Write the mark */
    fwrite(&assessmentp->mark, sizeof(int), 1, out_fp);

    /* Write the subject name */
    unsigned char len;
    len = strlen(assessmentp->subject);
    fwrite(&len, sizeof(unsigned char), 1, out_fp);
    fwrite(assessmentp->subject, sizeof(char), len, out_fp);

    /* Write the filename */
    len = strlen(assessmentp->filename);
    fwrite(&len, sizeof(unsigned char), 1, out_fp);
    fwrite(assessmentp->filename, sizeof(char), len, out_fp);
}

/*****************************************************************************
 * This function loops over a students assessment to be written.
 * Input:
 *   studentp - Pointer to the student whos assessment is to be written
 *   out_fp - File pointer to write the students assessments to.
 * Post:
 *   out_fp - Will have the assessments written to it.
*****************************************************************************/
void writeStudentAssessment(student_t *studentp, FILE *out_fp)
{
    /* Write number of assessments */
    fwrite(&studentp->assessments.size, sizeof(unsigned char), 1, out_fp);

    /* Write all assessments under student */
    int i;
    for (i = 0; i < studentp->assessments.size; i++)
    {
        writeAssessment(vectorGet(studentp->assessments, i), out_fp);
    }
}

/*****************************************************************************
 * This function writes a student to the file pointer.
 * Input:
 *   studentp - Pointer to the student who is to be written
 *   out_fp - File pointer to write the student to.
 * Post:
 *   out_fp - Will have the student written to it.
*****************************************************************************/
void writeStudent(student_t *studentp, FILE *out_fp)
{
    /* Write the studentId */
    fwrite(&studentp->studentId, sizeof(int), 1, out_fp);

    /* Write the student first name */
    unsigned char len;
    len = strlen(studentp->firstName);
    fwrite(&len, sizeof(unsigned char), 1, out_fp);
    fwrite(studentp->firstName, sizeof(char), len, out_fp);

    /* Write the student last name */
    len = strlen(studentp->lastName);
    fwrite(&len, sizeof(unsigned char), 1, out_fp);
    fwrite(studentp->lastName, sizeof(char), len, out_fp);

    /* Write student assessments */
    writeStudentAssessment(studentp, out_fp);
}

/*****************************************************************************
 * This function writes the actual contents of an assessment file to a file
 * pointer.
 * Input:
 *   filename - Pointer to the student who is to be written
 *   out_fp - File pointer to write the student to.
 * Post:
 *   out_fp - Will have the student written to it.
*****************************************************************************/
void writeFileContents(char *filename, FILE *out_fp)
{
    /* Open the file and determine its size. Write its size to the file */
    FILE *fp;
    fp = fopen(filename, "rb");
    fseek(fp, 0L, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);
    fwrite(&fileSize, sizeof(long), 1, out_fp);

#ifdef DEBUG
    printf("WRITE file size: %ld\n", fileSize);
#endif

    /* Now write each byte in the given file to the file pointer. */
    int buffer;
    while ((buffer = fgetc(fp)) != EOF)
    {
        fputc(buffer, out_fp);
    }
    fclose(fp);
}

/*****************************************************************************
 * This function writes the actual contents of a file_t to a file
 * pointer.
 * Input:
 *   file - file_t struct to be written to the file.
 *   out_fp - File pointer to write to.
 * Post:
 *   out_fp - Will have the file written to it.
*****************************************************************************/
void writeFileTContents(file_t file, FILE *out_fp)
{
#ifdef DEBUG
    printf("WRITE file size: %ld\n", file.size);
#endif

    /* Write the file size and the actual bytes of the file. */
    fwrite(&file.size, sizeof(long), 1, out_fp);
    fwrite(&file.bytes, sizeof(char), file.size, out_fp);
}

/*****************************************************************************
 * This function compares a file_t and a filename to determine if they match.
 * Input:
 *   ap - Pointer to a file_t struct.
 *   bp - Pointer to a string of a filename.
 * Return:
 *   0 - The strings are equal.
 *   non-zero - The string are not equal.
*****************************************************************************/
int compareFilenames(const void *ap, const void *bp)
{
    /* Type cast both void pointers to their types. */
    file_t *fileA = (file_t *)ap;
    char *filename = (char *)bp;

    /* String compare the names to see if they are equal. */
    return strcmp(fileA->filename, filename);
}

/*****************************************************************************
 * This function writes the actual assesment files.
 * Input:
 *   out_fp - File pointer to write to.
 *   studentList - The list of students and their assessments.
 *   existingFiles - The file existing in the database already.
 * Return:
 *   The file pointer out_fp will now contain the content of the assessment
 *   files.
*****************************************************************************/
void writeFiles(FILE *out_fp, vector_t studentList, vector_t existingFiles)
{
#ifdef DEBUG
    printf("Start write files\n");
#endif

    /* Loop over the students in the list. */
    int i, j;
    for (i = 0; i < studentList.size; i++)
    {
        /* Get the student and loop over their assessments. */
        student_t *student = vectorGet(studentList, i);
        for (j = 0; j < student->assessments.size; j++)
        {
            assessment_t *assessment = vectorGet(student->assessments, j);

#ifdef DEBUG
            printf("Writing file: %s\n", assessment->filename);
#endif

            /* Check if file exists in existing files */
            int pos = search(existingFiles, compareFilenames,
                             assessment->filename);
            if (pos == -1)
            {
                /* If the file does not exist in the database already,
                   write the file using the file in the drive. */
                writeFileContents(assessment->filename, out_fp);
            }
            else
            {
                /* Otherwise, write it from the existing database files. */
                file_t *file = vectorGet(existingFiles, pos);
                writeFileTContents(*file, out_fp);
            }
        }
    }
}

/*****************************************************************************
 * This function writes file header. This includes the student details and
 * their assessment details.
 * Input:
 *   out_fp - File pointer to write to.
 *   studentList - The list of students and their assessments.
 * Return:
 *   The file pointer out_fp will now contain the students and their 
 *   assessments information.
*****************************************************************************/
void writeHeader(FILE *out_fp, vector_t studentList)
{
#ifdef DEBUG
    printf("Start write header\n");
#endif

    /* Write number of students */
    fwrite(&studentList.size, sizeof(unsigned char), 1, out_fp);

    /* Loop over each student and write their details. */
    int i;
    for (i = 0; i < studentList.size; i++)
    {
        writeStudent(vectorGet(studentList, i), out_fp);
    }
}

/*****************************************************************************
 * This function XOR encrypts contents of a file using a temporary file. 
 * It also asks the user for the password to encrypt with.
 * Input:
 *   database_fp - File pointer to encrypt.
 *   startPos - Position to start the encryption from.
 * Return:
 *   0 - Success
 *   1 - Failure
*****************************************************************************/
int XOREncryptDatabase(FILE *database_fp, long startPos)
{
    /* Create a temp file to store the encrypted bytes temporary */
    FILE *temp_fp;
    temp_fp = fopen("encrypted.bin.tmp", "wb+");
    if (temp_fp == NULL)
    {
        printf("Error creating temp file");
        return 1;
    }

    /* Goto where the database files begin */
    fseek(database_fp, startPos, SEEK_SET);

    /* Ask the user for the password encrypt with */
    printf("Enter the password to encrypt with>");
    char key[255];
    scanf("%s", key);

    /* Get the hashed key for encrypting the 
           database using encrypt initial conditions */
    char hashed_key[HASH_SIZE];
    secureHashEncrypt(key, hashed_key);

    /* XOR encrypt database to a temp file */
    XOR_cipher(database_fp, temp_fp, hashed_key);

    /* Goto where the database files begin */
    fseek(database_fp, startPos, SEEK_SET);

    /* Get and write the hashed password 
           using password check initial conditions */
    secureHashPasswordCheck(key, hashed_key);
    fwrite(hashed_key, sizeof(char), HASH_SIZE, database_fp);

    /* Now copy the encrypted temp database files to the database */
    fseek(temp_fp, 0, SEEK_SET);
    copyFile(database_fp, temp_fp);

    /* Close the temp file and remove it. */
    fclose(temp_fp);
    remove("encrypted.bin.tmp");

    return 0;
}

/*****************************************************************************
 * This function Shift encrypts contents of a file using a temporary file. 
 * Input:
 *   database_fp - File pointer to encrypt.
 *   startPos - Position to start the encryption from.
 * Return:
 *   0 - Success
 *   1 - Failure
*****************************************************************************/
int shiftEncryptDatabase(FILE *database_fp, long startPos)
{
    /* Create a temp file to store the encrypted bytes temporary */
    FILE *temp_fp;
    temp_fp = fopen("encrypted.bin.tmp", "wb+");
    if (temp_fp == NULL)
    {
        printf("Error creating temp file");
        return 1;
    }

    /* Goto where the database files begin */
    fseek(database_fp, startPos, SEEK_SET);

    /* Shift encrypt database to a temp file */
    shift_encrypt(database_fp, temp_fp);

    /* Now copy the encrypted temp database files to the database */
    fseek(database_fp, startPos, SEEK_SET);
    fseek(temp_fp, 0, SEEK_SET);
    copyFile(database_fp, temp_fp);

    /* Close the temp file and remove it. */
    fclose(temp_fp);
    remove("encrypted.bin.tmp");

    return 0;
}

/*****************************************************************************
 * This function compressions the contents of a file using huffman encoding 
 * via a temporary file. Because the size is expected to differ, it replaces
 * the existing file by renaming it.
 * Input:
 *   database_fpp - Pointer to a file pointer to encrypt. As we need to swap
 *                  it with the new file pointer.
 *   startPos - Position to start the encryption from.
 *   outFile - The name of the file to swap it with.
 * Return:
 *   0 - Success
 *   1 - Failure
*****************************************************************************/
int huffmanCompressDatabase(FILE **database_fpp, long startPos,
                            char *outFile)
{
    /* Create a temp file to store the compressed bytes */
    FILE *temp_fp;
    temp_fp = fopen("compressed.bin.tmp", "wb+");
    if (temp_fp == NULL)
    {
        printf("Error creating temp file");
        return 1;
    }

    /* Goto where the database files begin */
    fseek(*database_fpp, 0, SEEK_SET);
    copyHeader(temp_fp, *database_fpp);
    /* Goto where the database files begin */
    fseek(*database_fpp, startPos, SEEK_SET);
    fseek(temp_fp, startPos, SEEK_SET);

    /* Compress the database to a temp file */
    huffmanCompress(*database_fpp, temp_fp);

    /* Now swap the compressed database with the existing database
       by closing the existing, removing and renaming it. */
    fclose(*database_fpp);
    *database_fpp = temp_fp;
    remove(outFile);
    rename("compressed.bin.tmp", outFile);

    return 1;
}

/*****************************************************************************
 * This function writes the entire database using the students list, new 
 * files and the files already existing in the database..
 * Input:
 *   studentList - List of students and their assessments.
 *   outFile - The name of the file to swap it with.
 *   bitFlag - Bit flag containing options for encryptions and compression.
 *   existingFiles - List of all the files existing in the database already.
 * Return:
 *   0 - Success
 *   1 - Failure
*****************************************************************************/
int writeDatabase(vector_t studentList, char *outFile, char bitFlag,
                  vector_t existingFiles)
{
#ifdef DEBUG
    printf("Start write database\n");
#endif

    FILE *out_fp;
    out_fp = fopen(outFile, "wb+");
    if (out_fp == NULL)
        return 1;

    /* Write all database information */
    writeHeader(out_fp, studentList);

    /* Write the bit flag */
    fwrite(&bitFlag, sizeof(char), 1, out_fp);

    /* Get pos of start of files */
    long file_pos = ftell(out_fp);

    /* Write files to be saved to the database */
    writeFiles(out_fp, studentList, existingFiles);

    /* If user has opted huffman compress, compress database */
    if (bitFlag & HUFFMAN_COMPRESS)
        huffmanCompressDatabase(&out_fp, file_pos, outFile);

    /* If user has opted run length compress, compress database */
    if (bitFlag & RUN_COMPRESS)
        ;

    /* If user has opted shift encryption, encrypt database */
    if (bitFlag & SHIFT_ENCRYPT)
        shiftEncryptDatabase(out_fp, file_pos);

    /* If user has opted XOR encryption, encrypt database */
    if (bitFlag & XOR_ENCRYPT)
        XOREncryptDatabase(out_fp, file_pos);

    fclose(out_fp);

    return 0;
}

/*****************************************************************************
 * This function reads an assessments informatiom a file pointer.
 * Input:
 *   assessmentp - Pointer to the assessment to be read to.
 *   in_fp - File pointer to read the assessment from.
 * Post:
 *   assessmentp will contain the assessment information.
*****************************************************************************/
void readAssessment(assessment_t *assessmentp, FILE *in_fp)
{
    /* Read the mark */
    fread(&assessmentp->mark, sizeof(int), 1, in_fp);

    /* Read the subject name */
    unsigned char len;
    fread(&len, sizeof(unsigned char), 1, in_fp);
    fread(assessmentp->subject, sizeof(char), len, in_fp);
    assessmentp->subject[len] = '\0';

    /* Read the filename */
    fread(&len, sizeof(unsigned char), 1, in_fp);
    fread(assessmentp->filename, sizeof(char), len, in_fp);
    assessmentp->filename[len] = '\0';
}

/*****************************************************************************
 * This function reads all the assessments under a student.
 * Input:
 *   studentp - Pointer to the student whos assessments are being read.
 *   in_fp - File pointer to read the assessments from.
 * Post:
 *   studentp will contain the all their assessment information.
*****************************************************************************/
void readStudentAssessment(student_t *studentp, FILE *in_fp)
{
    /* Read number of assessments */
    unsigned char num_assessments;
    fread(&num_assessments, sizeof(unsigned char), 1, in_fp);

    /* Loop over and read all assessments. */
    int i;
    for (i = 0; i < num_assessments; i++)
    {
        /* Read the assessment. */
        assessment_t assessment;
        readAssessment(&assessment, in_fp);

#ifdef DEBUG
        printf("READ assessment file: %s\n", assessment.filename);
#endif

        /* Push the assessment into the student. */
        vectorPushBack(&studentp->assessments, &assessment);
    }
}

/*****************************************************************************
 * This function reads a students information.
 * Input:
 *   studentp - Pointer to the student being read.
 *   in_fp - File pointer to read the student from.
 * Post:
 *   studentp will contain the all their information and their assessments.
*****************************************************************************/
void readStudent(student_t *studentp, FILE *in_fp)
{
    /* Write the studentId */
    fread(&studentp->studentId, sizeof(int), 1, in_fp);

    /* Read the student first name */
    unsigned char len;
    fread(&len, sizeof(unsigned char), 1, in_fp);
    fread(studentp->firstName, sizeof(char), len, in_fp);
    studentp->firstName[len] = '\0';

    /* Read the student last name */
    fread(&len, sizeof(unsigned char), 1, in_fp);
    fread(studentp->lastName, sizeof(char), len, in_fp);
    studentp->lastName[len] = '\0';

    /* Read student assessments */
    readStudentAssessment(studentp, in_fp);
}

/*****************************************************************************
 * This function reads the entire header from a file. This includes all 
 * students and their assessment information.
 * Input:
 *   database_fp - File pointer to the database to be read from.
 *   studentList - List to read the students to.
 * Post:
 *   studentList will contain all the students and their information from
 *   the database.
*****************************************************************************/
void readHeader(FILE *database_fp, vector_t *studentList)
{
    /* Read number of students */
    unsigned char numStudents;
    fread(&numStudents, sizeof(unsigned char), 1, database_fp);

#ifdef DEBUG
    printf("READ File count: %d\n", num_students);
#endif

    /* Loop over the number of students and read each one. */
    int i;
    for (i = 0; i < numStudents; i++)
    {
        /* Initialise and read the student. */
        student_t student;
        initStudent(&student);
        readStudent(&student, database_fp);

#ifdef DEBUG
        printf("READ Student ID: %d\n", student.studentId);
#endif

        /* Push the student just read onto the studentList. */
        vectorPushBack(studentList, &student);
    }
}

/*****************************************************************************
 * This function initialises a studentList and then reads the header of a 
 * database into that studentList.
 * Input:
 *   database_fp - File pointer to the database to be read from.
 *   studentList - List to read the students to.
 * Post:
 *   studentList will be initialised and will contain all the students and 
 *   their information from the database.
*****************************************************************************/
void readDatabaseFp(FILE *database_fp, vector_t *studentList)
{
    /* Initialise the student list. */
    /* TODO: This was sizeof(char) * 255 */
    initVector(studentList, sizeof(student_t));
    /* Read all the database information. */
    readHeader(database_fp, studentList);
}

/*****************************************************************************
 * This function reads a file by creating a file pointer then calls 
 * readDatabaseFp() to read it.
 * Input:
 *   databaseFile - The name of the file to be opened and read.
 *   studentList - List to read the students to.
 * Post:
 *   studentList will contain all the students and their information from
 *   the database.
 * Return:
 *   0 - Success
 *   1 - Failure
*****************************************************************************/
int readDatabase(char *databaseFile, vector_t *studentList)
{
    /* Create a file pointer to the database file. */
    FILE *database_fp;
    database_fp = fopen(databaseFile, "rb");
    if (database_fp == NULL)
        return 1;

    /* Read all the database information. */
    readDatabaseFp(database_fp, studentList);

    return 0;
}

/*****************************************************************************
 * This function reads a database file to memory using the file_t struct.
 * Input:
 *   databaseFile - The name of the file to be opened and read.
 *   files - List of file_t to store the file information to.
 * Post:
 *   files - Will contain a file_t for each file in databaseFile database.
 * Return:
 *   0 - Success
 *   1 - Failure
*****************************************************************************/
int readDatabaseToMemory(char *databaseFile, vector_t *files)
{
    /* Create a new database and read all the student information into list */
    FILE *database_fp;
    vector_t studentList;
    database_fp = fopen(databaseFile, "rb");
    readDatabaseFp(database_fp, &studentList);

    /* Retrieve just the filenames from the previously read student list. */
    vector_t filenames;
    getAllFilenames(studentList, &filenames);

    /* Unpackage all database files to a temp file
       i.e. decompress and decrypt them into temp file. */
    if (unpackageDatabaseFilesContents(database_fp, TEMP_DATABASE_NAME))
    {
        fclose(database_fp);
        return 1;
    }

    /* Open the unpackaged temp database file. */
    FILE *databaseTemp_fp;
    databaseTemp_fp = fopen(TEMP_DATABASE_NAME, "rb");
    {
        return 1;
    }

    /* Separate the content of the temp database file into 
       a list of file_t. */
    separateFilesToMemory(databaseTemp_fp, filenames, files);

    /* Close and remove temp database file. */
    fclose(databaseTemp_fp);
    remove(TEMP_DATABASE_NAME);

    return 0;
}

/*****************************************************************************
 * This function separates files in a database into list of file_t in memory
 * Input:
 *   database_fp - File pointer to the database to be read from.
 *   filenames - List of the file names to be read in the database.
 *   files - List of file_t to store the file information to.
 * Post:
 *   files - Will contain a file_t for each file in database_fp database 
 *           file pointer.
*****************************************************************************/
void separateFilesToMemory(FILE *database_fp, vector_t filenames, vector_t *files)
{
    /* Loop over each file in the filename list */
    int i;
    for (i = 0; i < filenames.size; i++)
    {
        /* Read the size of the current file being read. */
        file_t file;
        fread(&file.size, sizeof(long), 1, database_fp);

#ifdef DEBUG
        printf("To memory file size: %ld\n", file.size);
#endif

        /* Set the filename using the name from the list of filenames. */
        strcpy(file.filename, (char *)vectorGet(filenames, i));

#ifdef DEBUG
        printf("To memory READ %s size: %ld\n", file.filename, file.size);
#endif

        /* Read the bytes of the size read into file.bytes. 
           i.e. read the file contents into memory. */
        file.bytes = malloc(sizeof(char) * file.size);
        fread(file.bytes, sizeof(char), file.size, database_fp);
    }
}

/*****************************************************************************
 * This function separates the packaged files in a database by rewriting 
 * each one to a new file.
 * Input:
 *   database_fp - File pointer to the database to be read from.
 *   filenames - List of the file names to be read in the database.
 * Post:
 *   The packaged files in database_fp will be created in the current
 *   directory.
*****************************************************************************/
void separateFiles(FILE *database_fp, vector_t filenames)
{
    /* Loop over each file in the filename list */
    int i;
    for (i = 0; i < filenames.size; i++)
    {
        /* Read the size of the current file being read. */
        long fileSize;
        fread(&fileSize, sizeof(long), 1, database_fp);

        /* Copy the filename from the list so that
           it is easier to work with. */
        char filename[MAX_FILENAME_SIZE];
        strcpy(filename, (char *)vectorGet(filenames, i));

#ifdef DEBUG
        printf("READ %s size: %ld\n", filename, fileSize);
#endif

        /* Create (or open and overwrite) the filename from the list */
        FILE *file_fp = fopen(filename, "wb+");
        if (file_fp == NULL)
        {
            printf("error opening file %s\n", filename);
        }

        /* Now read each byte into the new file from the database. */
        int buffer;
        while (fileSize--)
        {
            buffer = fgetc(database_fp);
            fputc(buffer, file_fp);
        }

        /* Close the new file. */
        fclose(file_fp);
    }
}

/*****************************************************************************
 * This function separates the packaged files and searches for the filename
 * filter and rewriting that file only.
 * Input:
 *   database_fp - File pointer to the database to be read from.
 *   filenames - List of the file names to be read in the database.
 *   filenameFilter - Filename to rewrite from database.
 * Post:
 *   The packaged files in database_fp will be created in the current
 *   directory.
*****************************************************************************/
void separateFilesFilter(FILE *database_fp, vector_t filenames, char *filenameFilter)
{
    /* Loop over each file in the filename list */
    int i;
    for (i = 0; i < filenames.size; i++)
    {
        /* Read the size of the current file being read. */
        long fileSize;
        fread(&fileSize, sizeof(long), 1, database_fp);

        /* Copy the filename from the list so that
           it is easier to work with. */
        char filename[MAX_FILENAME_SIZE];
        strcpy(filename, (char *)vectorGet(filenames, i));

#ifdef DEBUG
        printf("READ %s size: %ld\n", filename, fileSize);
#endif

        /* Check if filename equals the one wanted. */
        if (!strcmp(filename, filenameFilter))
        {
            /* Create (or open and overwrite) the filename from the list */
            FILE *file_fp = fopen(filename, "wb+");
            if (file_fp == NULL)
            {
                printf("error opening file %s\n", filename);
            }

            /* Now read each byte into the new file from the database. */
            int buffer;
            while (fileSize--)
            {
                buffer = fgetc(database_fp);
                fputc(buffer, file_fp);
            }

            /* Close the new file. */
            fclose(file_fp);
        }
        else
        {
            /* If it doesn't, still need to move 
               the file pointer to next file. */
            while (fileSize--)
                fgetc(database_fp);
        }
    }
}

/*****************************************************************************
 * This function extracts all assessment files to the current directory. 
 * This includes unpackaging (decrypt and decompress) the assessment files
 * before extraction.
 * Input:
 *   databaseFile - The name of the database to extract the file from.
 *   filenameFilter - Filename to rewrite from database. If NULL filter
 *                    is ignored.
 * Post:
 *   The packaged files in database_fp will be created in the current
 *   directory.
*****************************************************************************/
int unpackageDatabaseFiles(char *databaseFile, char *filenameFilter)
{
    /* Open the database file. */
    FILE *database_fp;
    database_fp = fopen(databaseFile, "rb");
    if (database_fp == NULL)
    {
        printf("Cannot read database: %s\n", databaseFile);
    }

    /* Read the student list to get all the filenames. */
    vector_t studentList;
    readDatabaseFp(database_fp, &studentList);
    vector_t filenames;
    getAllFilenames(studentList, &filenames);

    /* Unpackage the database files i.e. decrypt and/or decompress them */
    if (unpackageDatabaseFilesContents(database_fp, TEMP_DATABASE_NAME))
    {
        return 1;
    }

    /* Unpackage the database files i.e. decrypt and/or decompress them */
    FILE *files_fp = fopen(TEMP_DATABASE_NAME, "rb");
    if (database_fp == NULL)
    {
        printf("There was a problem reading the unpackaged database.\n");
        return 1;
    }

    /* Now separate the grouped files into new files/file. */
    if (filenameFilter == NULL)
        separateFiles(files_fp, filenames);
    else
        separateFilesFilter(files_fp, filenames, filenameFilter);

    /* close the unpackaged database and remove it. */
    fclose(files_fp);
    remove(TEMP_DATABASE_NAME);
    return 0;
}

/*****************************************************************************
 * This function XOR decrypts contents of a file using a temporary file. 
 * It also asks the user for the password to decrypt with and ensure it is
 * the correct password.
 * Input:
 *   database_fpp - Pointer to a file pointer to decrypt.
 *   outFile - The name of the file to replace with the decrypted version.
 * Return:
 *   0 - Success
 *   1 - Failure
*****************************************************************************/
int XORDecryptDatabase(FILE **database_fpp, char *outFile)
{
    /* Create a temp file to store the decrypted bytes temporary */
    FILE *temp_fp;
    char new_name[] = "decrypt.bin.tmp";
    temp_fp = fopen(new_name, "wb+");
    if (temp_fp == NULL)
    {
        printf("Error creating temp file");
        return 1;
    }

    /* Goto where the database files begin */
    fseek(*database_fpp, 0, SEEK_SET);

    printf("This database is encrypted\nEnter you password>");
    char key[255];
    scanf("%s", key);

    /* Get the hashed key for password checking.
           Compare it with the one in the database. 
           This will tell us if the user has entered
           The correct password. */
    char hashed_key[HASH_SIZE];
    secureHashPasswordCheck(key, hashed_key);
    char stored_hash_key[HASH_SIZE];
    fread(stored_hash_key, sizeof(unsigned char), HASH_SIZE, *database_fpp);
    if (strcmp(hashed_key, stored_hash_key) != 0)
    {
        printf("You have entered the wrong password.\n");
        return 1;
    }
    printf("You have entered the correct password.\n");

    /* Decrypt the database to a temp file.
           Using the hashed encrypt password. */
    secureHashEncrypt(key, hashed_key);
    XOR_cipher(*database_fpp, temp_fp, hashed_key);

    /* Swap the existing database with the temp database.
       By removing it and changing the pointer. */
    fclose(*database_fpp);
    remove(outFile);
    *database_fpp = temp_fp;
    rename(new_name, outFile);
    return 0;
}

/*****************************************************************************
 * This function shift decrypts contents of a file using a temporary file.
 * Input:
 *   database_fpp - Pointer to a file pointer to decrypt.
 *   outFile - The name of the file to replace with the decrypted version.
 * Return:
 *   0 - Success
 *   1 - Failure
*****************************************************************************/
int shiftDecryptDatabase(FILE **database_fpp, char *outFile)
{
    /* Create a temp file to store the decrypted bytes temporary */
    FILE *temp_fp;
    char new_name[] = "decrypt.bin.tmp";
    temp_fp = fopen(new_name, "wb+");
    if (temp_fp == NULL)
    {
        printf("Error creating temp file");
        return 1;
    }

    /* Goto where the database files begin */
    fseek(*database_fpp, 0, SEEK_SET);

    /* Decrypt the database to a temp file. */
    shift_decrypt(*database_fpp, temp_fp);

    /* Swap the existing database with the temp database.
       By removing it and changing the pointer. */
    fclose(*database_fpp);
    remove(outFile);
    *database_fpp = temp_fp;
    rename(new_name, outFile);
    return 0;
}

/*****************************************************************************
 * This function huffman decompresses the contents of a file using a 
 * temporary file.
 * Input:
 *   database_fpp - Pointer to a file pointer to decompress.
 *   outFile - The name of the file to replace with the decrypted version.
 * Return:
 *   0 - Success
 *   1 - Failure
*****************************************************************************/
int huffmanDecompressDatabase(FILE **database_fpp, char *outFile)
{
    /* Create a temp file to store the decompressed bytes temporary */
    FILE *temp_fp;
    char new_name[] = "decompress.bin.tmp";
    temp_fp = fopen(new_name, "wb+");
    if (temp_fp == NULL)
    {
        printf("Error creating temp file");
        return 1;
    }

    /* Goto where the database files begin */
    fseek(*database_fpp, 0, SEEK_SET);

    /* Decompress the database to a temp file. */
    huffmanDecompress(*database_fpp, temp_fp);

    /* Swap the existing database with the temp database.
       By removing it and changing the pointer. */
    fclose(*database_fpp);
    remove(outFile);
    *database_fpp = temp_fp;
    rename(new_name, outFile);
    return 0;
}

/*****************************************************************************
 * This function does the unpackaging to the database by determining if it
 * requires decryption and/or decompression.
 * Input:
 *   database_fp - File pointer to the database to be read from.
 *   files - Filename to write the unpackaged files to.
 * Post:
 *   The files will be written decrypted and/or decompressed to filename of
 *   files.
*****************************************************************************/
int unpackageDatabaseFilesContents(FILE *database_fp, char *files)
{
    /* Read the bit flag */
    char bitFlag;
    fread(&bitFlag, sizeof(char), 1, database_fp);

#ifdef DEBUG
    printf("bitFlag: %d\n", bitFlag);
#endif

    /* Duplicate database for unpackaging */
    FILE *files_fp;
    files_fp = fopen(files, "wb+");
    copyFile(files_fp, database_fp);
    fclose(database_fp);

    /* If user has opted XOR encryption, unencrypt database */
    if (bitFlag & XOR_ENCRYPT)
        XORDecryptDatabase(&files_fp, files);

    /* If user has opted shift encryption, unencrypt database */
    if (bitFlag & SHIFT_ENCRYPT)
        shiftDecryptDatabase(&files_fp, files);

    /* If the database has been compressed using huffman, decompress database */
    if (bitFlag & HUFFMAN_COMPRESS)
        ;

    /* If the database has been compressed using huffman, decompress database */
    if (bitFlag & HUFFMAN_COMPRESS)
        huffmanDecompressDatabase(&files_fp, files);

    fclose(files_fp);
    return 0;
}