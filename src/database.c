#include <stdio.h>
#include <string.h>
#include "database.h"
#include "vector.h"
#include "encrypt.h"
#include "compress.h"
#include "secure_hash.h"
#include "student.h"

void read_header(FILE *database_fp, vector_t *filenames);

void print_filenames(int index, void *value)
{
    printf("[%d] %s\n", index, (char *)value);
}

void copy_file(FILE *dest, FILE *src)
{
    int buffer;
    while ((buffer = getc(src)) != EOF)
    {
        putc(buffer, dest);
    }
}

void copy_header(FILE *dest, FILE *src)
{
    /* Seek the end of the header */
    vector_t temp;
    init_vector(&temp, 10, sizeof(char) * 255);
    read_header(src, &temp);
    free_vector(temp);
    char bit_flag;
    fread(&bit_flag, sizeof(char), 1, src);

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

void write_assessment(assessment_t *assessment, FILE *out_fp)
{
    /* Write the mark */
    fwrite(&assessment->mark, sizeof(int), 1, out_fp);

    /* Write the subject name */
    unsigned char len;
    len = strlen(assessment->subject);
    fwrite(&len, sizeof(unsigned char), 1, out_fp);
    fwrite(assessment->subject, sizeof(char), len, out_fp);

    /* Write the filename */
    len = strlen(assessment->filename);
    fwrite(&len, sizeof(unsigned char), 1, out_fp);
    fwrite(assessment->filename, sizeof(char), len, out_fp);
}

void write_student_assessment(student_t *student, FILE *out_fp)
{
    /* Write number of assessments */
    fwrite(&student->assessments.size, sizeof(unsigned char), 1, out_fp);

    /* Write assessments */
    int i;
    for (i = 0; i < student->assessments.size; i++)
    {
        write_assessment(vector_get(student->assessments, i), out_fp);
    }
}

void write_student(student_t *student, FILE *out_fp)
{
    /* Write the studentId */
    fwrite(&student->studentId, sizeof(int), 1, out_fp);

    /* Write the student first name */
    unsigned char len;
    len = strlen(student->firstName);
    fwrite(&len, sizeof(unsigned char), 1, out_fp);
    fwrite(student->firstName, sizeof(char), len, out_fp);

    /* Write the student last name */
    len = strlen(student->lastName);
    fwrite(&len, sizeof(unsigned char), 1, out_fp);
    fwrite(student->lastName, sizeof(char), len, out_fp);

    /* Write student assessments */
    write_student_assessment(student, out_fp);
}

int write_file_contents(char *filename, FILE *out_fp)
{
    FILE *fp;

    fp = fopen(filename, "rb");

    fseek(fp, 0L, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);

    printf("WRITE file size: %ld\n", file_size);

    fwrite(&file_size, sizeof(long), 1, out_fp);

    int buffer;
    while ((buffer = fgetc(fp)) != EOF)
    {
        fputc(buffer, out_fp);
    }

    fclose(fp);
}

int write_files(FILE *out_fp, vector_t student_list)
{
#ifndef DEBUG
    printf("Start write files\n");
#endif

    int i, j;
    for (i = 0; i < student_list.size; i++)
    {
        student_t *student = vector_get(student_list, i);
        for (j = 0; j < student->assessments.size; j++)
        {
            
            char filename[MAX_FILENAME_SIZE];
            assessment_t *assessment = vector_get(student->assessments, j);
            strcpy(filename, assessment->filename);
            
            #ifndef DEBUG
                printf("Writing file: \n");
            #endif

            write_file_contents(filename, out_fp);
        }
    }
}

void write_header(FILE *out_fp, vector_t student_list)
{
#ifndef DEBUG
    printf("Start write header\n");
#endif

    /* Write number of students */
    fwrite(&student_list.size, sizeof(unsigned char), 1, out_fp);

    /* Write students */
    int i;
    for (i = 0; i < student_list.size; i++)
    {
        write_student(vector_get(student_list, i), out_fp);
    }
}

int xor_encrypt_database(FILE *database_fp, long start_pos)
{
    FILE *temp_fp;
    temp_fp = fopen("encrypted.bin.tmp", "wb+");
    if (temp_fp == NULL)
    {
        printf("Error creating temp file");
        return 0;
    }

    /* Goto where the database files begin */
    fseek(database_fp, start_pos, SEEK_SET);

    printf("Enter the password to encrypt with>");
    char key[255];
    scanf("%s", key);

    /* Get the hashed key for encrypting the 
           database using encrypt initial conditions */
    char hashed_key[HASH_SIZE];
    secure_hash_encrypt(key, hashed_key);

    /* encrypt database to a temp file */
    XOR_cipher(database_fp, temp_fp, hashed_key);

    /* Goto where the database files begin */
    fseek(database_fp, start_pos, SEEK_SET);

    /* Get and write the hashed password 
           using password check initial conditions */
    int t = secure_hash_password_check(key, hashed_key);
    fwrite(hashed_key, sizeof(char), HASH_SIZE, database_fp);

    /* Now copy the encrypted temp database files to the database */
    fseek(temp_fp, 0, SEEK_SET);
    copy_file(database_fp, temp_fp);

    fclose(temp_fp);
    remove("encrypted.bin.tmp");
}

int shift_encrypt_database(FILE *database_fp, long start_pos)
{
    FILE *temp_fp;
    temp_fp = fopen("encrypted.bin.tmp", "wb+");
    if (temp_fp == NULL)
    {
        printf("Error creating temp file");
        return 0;
    }

    /* Goto where the database files begin */
    fseek(database_fp, start_pos, SEEK_SET);

    /* Encrypt database to a temp file */
    shift_encrypt(database_fp, temp_fp);

    /* Now copy the encrypted temp database files to the database */
    fseek(database_fp, start_pos, SEEK_SET);
    fseek(temp_fp, 0, SEEK_SET);
    copy_file(database_fp, temp_fp);

    fclose(temp_fp);
    remove("encrypted.bin.tmp");
}

int huffman_compress_database(FILE **database_fp, long start_pos, char *out_file)
{
    FILE *temp_fp;
    temp_fp = fopen("compressed.bin.tmp", "wb+");
    if (temp_fp == NULL)
    {
        printf("Error creating temp file");
        return 0;
    }

    /* Goto where the database files begin */
    fseek(*database_fp, 0, SEEK_SET);
    copy_header(temp_fp, *database_fp);
    /* Goto where the database files begin */
    fseek(*database_fp, start_pos, SEEK_SET);
    fseek(temp_fp, start_pos, SEEK_SET);

    /* Encrypt database to a temp file */
    compress(*database_fp, temp_fp);

    /* Now swap the compressed database with the existing database */
    fclose(*database_fp);
    *database_fp = temp_fp;
    remove(out_file);
    rename("compressed.bin.tmp", out_file);
}

int write_database(vector_t student_list, char *out_file, char bit_flag)
{
#ifndef DEBUG
    printf("Start write database\n");
#endif

    FILE *out_fp;
    out_fp = fopen(out_file, "wb+");
    if (out_fp == NULL)
        return 1;

    /* Write all database information */
    write_header(out_fp, student_list);

    /* Write bit_flag */
    fwrite(&bit_flag, sizeof(char), 1, out_fp);

    /* Get pos of start of files */
    long file_pos = ftell(out_fp);

    /* Write files to be saved to the database */
    write_files(out_fp, student_list);

    /* If user has opted XOR encryption, encrypt database */
    if (bit_flag & XOR_ENCRYPT)
        xor_encrypt_database(out_fp, file_pos);

    /* If user has opted shift encryption, encrypt database */
    if (bit_flag & SHIFT_ENCRYPT)
        shift_encrypt_database(out_fp, file_pos);

    /* If user has opted huffman compress, compress database */
    if (bit_flag & HUFFMAN_COMPRESS)
        huffman_compress_database(&out_fp, file_pos, out_file);

    /* If user has opted run length compress, compress database */
    if (bit_flag & RUN_COMPRESS)
        ;

    fclose(out_fp);

    return 0;
}

void read_assessment(assessment_t *assessment, FILE *in_fp)
{
    /* Write the mark */
    fread(&assessment->mark, sizeof(int), 1, in_fp);

    /* Write the subject name */
    unsigned char len;
    fread(&len, sizeof(unsigned char), 1, in_fp);
    fread(assessment->subject, sizeof(char), len, in_fp);
    assessment->subject[len] = '\0';

    /* Write the filename */
    fread(&len, sizeof(unsigned char), 1, in_fp);
    fread(assessment->filename, sizeof(char), len, in_fp);
    assessment->filename[len] = '\0';
}

void read_student_assessment(student_t *student, FILE *in_fp)
{
    unsigned char num_assessments;
    /* Read number of assessments */
    fread(&num_assessments, sizeof(unsigned char), 1, in_fp);

    /* Read assessments */
    int i;
    for (i = 0; i < num_assessments; i++)
    {
        assessment_t assessment;
        /* TODO: initAssessment... initStudent(&student); */
        read_assessment(&assessment, in_fp);

        #ifndef DEBUG
            printf("READ assessment file: %s\n", assessment.filename);
        #endif

        vector_push_back(&student->assessments, &assessment);
    }
}

void read_student(student_t *student, FILE *in_fp)
{
    /* Write the studentId */
    fread(&student->studentId, sizeof(int), 1, in_fp);

    /* Read the student first name */
    unsigned char len;
    fread(&len, sizeof(unsigned char), 1, in_fp);
    fread(student->firstName, sizeof(char), len, in_fp);
    student->firstName[len] = '\0';

    /* Write the student last name */
    fread(&len, sizeof(unsigned char), 1, in_fp);
    fread(student->lastName, sizeof(char), len, in_fp);
    student->lastName[len] = '\0';

    /* Write student assessments */
    read_student_assessment(student, in_fp);
}

void read_header(FILE *database_fp, vector_t *student_list)
{
    /* Read number of students */
    unsigned char num_students;
    fread(&num_students, sizeof(unsigned char), 1, database_fp);

    #ifndef DEBUG
        printf("READ File count: %d\n", num_students);
    #endif

    int i;
    for (i = 0; i < num_students; i++)
    {
        student_t student;
        initStudent(&student);
        read_student(&student, database_fp);

        #ifndef DEBUG
            printf("READ Student ID: %d\n", student.studentId);
        #endif

        vector_push_back(student_list, &student);
    }
}

int read_database_fp(FILE *database_fp, vector_t *filenames)
{
    /* Read all the database information */
    init_vector(filenames, 10, sizeof(char) * 255);
    read_header(database_fp, filenames);
}

int read_database(char *database_file, vector_t *filenames)
{
    FILE *database_fp;
    database_fp = fopen(database_file, "rb");
    read_database_fp(database_fp, filenames);
    /* print_vector(*filenames, print_filenames); */

    return 0;
}

int separate_files(FILE *database_fp, vector_t filenames, char *suffix)
{
    int i;
    for (i = 0; i < filenames.size; i++)
    {
        long file_size;
        fread(&file_size, sizeof(long), 1, database_fp);

        printf("file size: %ld\n", file_size);

        char filename[MAX_FILENAME_SIZE + 10];
        strcpy(filename, (char *)vector_get(filenames, i));
        strcat(filename, suffix);

#ifndef DEBUG
        printf("READ %s size: %ld\n", filename, file_size);
#endif

        FILE *file_fp = fopen(filename, "wb+");
        if (file_fp == NULL)
        {
            printf("error opening file\n");
        }
        int buffer;
        while (file_size--)
        {
            buffer = fgetc(database_fp);
            fputc(buffer, file_fp);
        }
        fclose(file_fp);
    }
}

int unpackage_database_files(char *database_file, char *dir)
{
    FILE *database_fp;
    database_fp = fopen(database_file, "rb");
    vector_t student_list;
    read_database_fp(database_fp, &student_list);
    vector_t filenames;
    getAllFilenames(student_list, &filenames);
    print_vector(filenames, print_filenames);

    /* Read the bit flag */
    char bit_flag;
    fread(&bit_flag, sizeof(char), 1, database_fp);

#ifndef DEBUG
    printf("bit_flag: %d\n", bit_flag);
#endif

    /* Duplicate database for unpackaging */
    FILE *files_fp;
    char files[] = "database.bin.tmp";
    files_fp = fopen(files, "wb+");
    copy_file(files_fp, database_fp);
    fclose(database_fp);

    /* If user has opted XOR encryption, unencrypt database */
    if (bit_flag & XOR_ENCRYPT)
    {
        FILE *temp_fp;
        char new_name[] = "decrypt.bin.tmp";
        temp_fp = fopen(new_name, "wb+");
        if (temp_fp == NULL)
        {
            printf("Error creating temp file");
            return 0;
        }

        /* Goto where the database files begin */
        fseek(files_fp, 0, SEEK_SET);

        printf("This database is encrypted\nEnter you password>");
        char key[255];
        scanf("%s", key);

        /* Get the hashed key for password checking.
           Compare it with the one in the database. 
           This will tell us if the user has entered
           The correct password. */
        char hashed_key[HASH_SIZE];
        secure_hash_password_check(key, hashed_key);
        unsigned char stored_hash_key[HASH_SIZE];
        fread(stored_hash_key, sizeof(unsigned char), HASH_SIZE, files_fp);

        if (strcmp(hashed_key, stored_hash_key) != 0)
        {
            printf("You have entered the wrong password.\n");
            return 1;
        }
        printf("You have entered the correct password.\n");

        /* Decrypt the database to a temp file.
           Using the hashed encrypt password. */
        secure_hash_encrypt(key, hashed_key);
        XOR_cipher(files_fp, temp_fp, hashed_key);

        fclose(files_fp);
        remove(files);
        files_fp = temp_fp;
        rename(new_name, files);
    }

    /* If user has opted shift encryption, unencrypt database */
    if (bit_flag & SHIFT_ENCRYPT)
    {
        FILE *temp_fp;
        char new_name[] = "decrypt.bin.tmp";
        temp_fp = fopen(new_name, "wb+");
        if (temp_fp == NULL)
        {
            printf("Error creating temp file");
            return 0;
        }

        /* Goto where the database files begin */
        fseek(files_fp, 0, SEEK_SET);

        /* Decrypt the database to a temp file. */
        shift_decrypt(files_fp, temp_fp);

        fclose(files_fp);
        remove(files);
        files_fp = temp_fp;
        rename(new_name, files);
    }

    /* If the database has been compressed using huffman, decompress database */
    if (bit_flag & HUFFMAN_COMPRESS)
    {
        FILE *temp_fp;
        char new_name[] = "decompress.bin.tmp";
        temp_fp = fopen(new_name, "wb+");
        if (temp_fp == NULL)
        {
            printf("Error creating temp file");
            return 0;
        }

        /* Goto where the database files begin */
        fseek(files_fp, 0, SEEK_SET);

        /* Decompress the database to a temp file. */
        decompress(files_fp, temp_fp);

        fclose(files_fp);
        remove(files);
        files_fp = temp_fp;
        rename(new_name, files);
    }

    fseek(files_fp, 0, SEEK_SET);
    separate_files(files_fp, filenames, dir);

    fclose(files_fp);
}

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

int createEmptyDatabase(char *filename)
{

    FILE *filep;
    if ((filep = fopen(filename, "wb")))
    {
        int size = 0;
        fwrite(&size, sizeof(int), 1, filep);
        fclose(filep);
        return 0;
    }
    return 1;
}