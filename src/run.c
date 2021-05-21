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

int main()
{

    FILE *in_fp = fopen("in", "rb");
    FILE *out_fp = fopen("out", "wb");

    compress(in_fp, out_fp);

    fclose(in_fp);
    fclose(out_fp);
    return 0;
}
