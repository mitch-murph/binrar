#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compress(FILE *in_fp, FILE *out_fp)
{
    int a = 0;
    int c = 1;

    int prev;
    int temp;
    int buffer;
    while ((buffer = fgetc(in_fp)) != EOF)
    {
        if (a == 0)
        {
            temp = buffer;
        }
        else
        {
            prev = temp;
            temp = buffer;
            if (prev == temp)
            {
                c++;
            }
            else
            {
                fputc(c, out_fp);
                fputc(prev, out_fp);
                c = 0;
            }
        }
        a++;
    }
    if (a > 0)
    {
        fputc(c, out_fp);
        fputc(prev, out_fp);
    }
    return a;
}

int decompress(FILE *in_fp, FILE *out_fp)
{
    int frequency;
    while ((frequency = fgetc(in_fp)) != EOF)
    {
        int value = fgetc(in_fp);
        int i;
        for (i = 0; i < frequency; i++)
            fputc(value, out_fp);
    }
}

int main()
{

    FILE *in_fp = fopen("out", "rb");
    FILE *out_fp = fopen("outout", "wb");

    decompress(in_fp, out_fp);

    fclose(in_fp);
    fclose(out_fp);
    return 0;
}
