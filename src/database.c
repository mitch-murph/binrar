#include <stdio.h>
#include <string.h>
#include "database.h"
#include "vector.h"

void print_filenames(int index, void *value)
{
    printf("[%d] %s\n", index, (char *)value);
}

int write_filename(char *filename, FILE *out_fp)
{
    /* TODO: force filename size */
    unsigned char len = strlen(filename);
    fwrite(&len, sizeof(unsigned char), 1, out_fp);
    fwrite(filename, sizeof(char), len, out_fp);
    return 0;
}

int write_file_contents(char *filename, FILE *out_fp)
{
    FILE *fp;

    fp = fopen(filename, "rb");

    fseek(fp, 0L, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);

    fwrite(&file_size, sizeof(long), 1, out_fp);

    int buffer;
    while ((buffer = fgetc(fp)) != EOF)
    {
        fputc(buffer, out_fp);
    }

    fclose(fp);
}

int combine_files(vector_t filenames, char *out_file, char bit_flag)
{
    FILE *out_fp;
    out_fp = fopen(out_file, "wb");

    fwrite(&filenames.size, sizeof(unsigned char), 1, out_fp);

    int i;
    for (i = 0; i < filenames.size; i++)
    {
        write_filename(vector_get(filenames, i), out_fp);
    }

    fwrite(&bit_flag, sizeof(char), 1, out_fp);

    for (i = 0; i < filenames.size; i++)
    {
        write_file_contents(vector_get(filenames, i), out_fp);
    }

    fclose(out_fp);
}

int read_filename(char *filename, FILE *in_fp)
{
    unsigned char len;
    fread(&len, sizeof(unsigned char), 1, in_fp);
    /* TODO: Read name size */
    fread(filename, sizeof(char), len, in_fp);
    return 0;
}

int separate_files(char *in_file, char * dir)
{
    vector_t filenames;
    init_vector(&filenames, 10, sizeof(char) * 255);


    FILE *in_fp;
    in_fp = fopen(in_file, "rb");

    unsigned char file_count;
    fread(&file_count, sizeof(unsigned char), 1, in_fp);

    printf("READ File count: %d\n", file_count);

    int i;
    for (i = 0; i < file_count; i++)
    {
        /* TODO: DEFINE filename_size */
        char filename[255];
        /*char new_[300];
        strcpy(new_, dir);*/
        read_filename(filename, in_fp);
        printf("cat filename: %s\n", filename);
        vector_push_back(&filenames, filename);
    }

    print_vector(filenames, print_filenames);
    char bit_flag;
    fread(&bit_flag, sizeof(char), 1, in_fp);

    for (i = 0; i < file_count; i++)
    {
        long file_size;
        fread(&file_size, sizeof(long), 1, in_fp);
        
        char fn[300];
        strcpy(fn, (char*)vector_get(filenames, i));
        strcat(fn, ".out");

        printf("READ %s size: %ld\n", fn, file_size);
        FILE *newp = fopen(fn, "wb");
        if (newp == NULL)
        {
            printf("error opening file\n");
        }
        int buffer;
        for (; file_size > 0; file_size--)
        {
            buffer = fgetc(in_fp);
            fputc(buffer, newp);
            printf("%02x ", buffer);
        }
        printf("\n");
        fclose(newp);
    }

    fclose(in_fp);
}

void seek_file_contents(FILE *fp)
{
    unsigned char file_count;
    fread(&file_count, sizeof(unsigned char), 1, fp);
    char filename[255];
    int i;
    for (i = 0; i < file_count; i++)
        read_filename(filename, fp);
}

void copy_file(FILE *dest, FILE *src)
{
    int buffer;
    while ((buffer = getc(src)) != EOF)
    {
        putc(buffer, dest);
    }
}