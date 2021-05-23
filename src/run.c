#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

/*****************************************************************************
 * This function compresses a file using run length compression
 * Input:
 *   in_fp - The file to be read and compressed.
 *   out_fp - The file to write the compressed file.
 * Post:
 *   The out_fp file will have the compressed version written.
*****************************************************************************/
void runLengthCompress(FILE *in_fp, FILE *out_fp)
{
    /* Loop over each byte in the file. */
    int flag = 0;
    int count = 1;
    int buffer, prev, temp;
    while ((buffer = fgetc(in_fp)) != EOF)
    {
        /* If its the first byte, there is no previous byte 
           to compare it with so it will just be put in temp. */
        if (flag == 0)
        {
            temp = buffer;
            flag = 1;
        }
        else
        {
            /* Rearrange the variables so that 
               prev contains the previous buffer
               and the current buffer will be in the prev next loop. */
            prev = temp;
            temp = buffer;
            if (prev == buffer)
            {
                /* If the previous matches the current, increment. */
                count++;
            }
            else
            {
                /* If it no longer matches the previous byte.
                   Write the count and the byte. 
                   Set count back to 0. */
                fputc(count, out_fp);
                fputc(prev, out_fp);
                count = 0;
            }
        }
    }
    /* Write the final byte and its size. Ensure the file is not empty. */
    if (flag != 0)
    {
        fputc(count, out_fp);
        fputc(prev, out_fp);
    }
}

/*****************************************************************************
 * This function decompresses a run length encoded file into another file.
 * Input:
 *   in_fp - The file pointer containing to be decompressed.
 *   out_fp - The file pointer to be written to.
 * Post:
 *   The out_fp will contain the decompressed contents.
*****************************************************************************/
void runLengthDecompress(FILE *in_fp, FILE *out_fp)
{
    /* Loop while not EOF */
    int frequency;
    while ((frequency = fgetc(in_fp)) != EOF)
    {
        /* Get the frequency and value */
        int value = fgetc(in_fp);

        /* Write the value, frequency times. */
        int i;
        for (i = 0; i < frequency; i++)
            fputc(value, out_fp);
    }
}