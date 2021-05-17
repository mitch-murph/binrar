#include <stdio.h>
#include <string.h>
#include "filepackager.h"
#include "vector.h"

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

int combine_files(vector_t filenames, char *out_file)
{
    FILE *out_fp;
    out_fp = fopen(out_file, "wb");

    fwrite(&filenames.size, sizeof(unsigned char), 1, out_fp);

    int i;
    for (i = 0; i < filenames.size; i++)
    {
        write_filename(vector_get(filenames, i), out_fp);
    }

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

int separate_files(char *in_file)
{
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
        read_filename(filename, in_fp);
        printf("READ Filename is: %s\n", filename);
    }

    for (i = 0; i < file_count; i++)
    {
        long file_size;
        fread(&file_size, sizeof(long), 1, in_fp);

        printf("READ File size: %ld\n", file_size);

        int buffer;
        for (; file_size > 0; file_size--)
        {
            buffer = fgetc(in_fp);
            printf("%02x ", buffer);
        }
        printf("\n");
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