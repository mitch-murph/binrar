#include "compress.h"
#include "bit_array.h"
#include "filepackager.h"
#include <stdio.h>

void read_bits()
{
    FILE *fp;
    int buffer;
    int temp;
    fp = fopen("data-files/write.bin", "rb");

    int count = 0, flag = 1;
    while (1)
    {
        if (flag)
        {
            if ((buffer = getc(fp)) == EOF)
            {
                break;
            }
        }
        int i = 0;
        if (!flag)
            i = count;
        flag = 1;
        for (; i < 8; i++)
        {
            if (get_bit(buffer, i))
            {
                i++;
                temp = buffer;
                buffer = getc(fp);
                int c = combine_bits(temp, buffer, i);
                printf("\n[%02x] ", (unsigned char)c);
                print_bits(c);
                flag = 0;
                count = i;
                break;
            }
        }
    }

    fclose(fp);
}

int main(int argc, char **argv)
{

    /*
    FILE *fp3 = fopen("data-files/comp.bin", "rb");
    int i;
    int buffer = 0, buffer_count = 0;
    for (i = 0; i < 8; i++)
    {
        printf("[%d]\t%d %d\n", i, buffer_count, read_n_bit(&buffer, &buffer_count, 7, fp3));
    }
    fclose(fp3);
    */

    FILE *fp1 = fopen("data-files/comp.bin", "rb");
    FILE *fp2 = fopen("data-files/out.bin", "wb");
    compress(fp1, fp2);
    fclose(fp1);
    fclose(fp2);


    FILE *fp3 = fopen("data-files/out.bin", "rb");
    decompress(fp3, fp3);
    fclose(fp3);
    return 0;
}

/*
001A1C01E01D1B

28 00101000
34 00110100
35 00110101
15 00010101
44 01000100
A1 10100001
00 00000000

28 00101000
34 00110100
35 00110101
15 00010101
44 01000100
a1 10100001
00 00000000

001 01000001 1 01000011 01 01000101 01 01000100 1 0100001 00000000
00101000 00110100  00110101  00010101  01000100 10100001  00000000
2834351544A100
*/