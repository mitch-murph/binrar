#include "compress.h"
#include "bit_array.h"
#include "filepackager.h"
#include <stdio.h>


void generate_bin()
{
    FILE *fp;
    fp = fopen("data-files/comp2.bin", "wb");

    int a[] = {0x28, 0x34, 0x35, 0x15, 0x44, 0xA1, 0x00, 0xffff};
    int i;
    for (i = 0; a[i] != 0xffff; i++)
    {
        fwrite(&a[i], sizeof(char), 1, fp);
    }

    fclose(fp);
}

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
    FILE *fp1 = fopen("data-files/comppp.bin", "rb");
    FILE *fp2 = fopen("data-files/out.bin", "wb");
    compress(fp1, fp2);
    fclose(fp1);
    fclose(fp2);

    return 0;
}