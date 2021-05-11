#include "compress.h"
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

int get_bit(int v, int i)
{
    return (v >> (7 - i)) & 1;
}

int combine_bits(int a, int b, int s)
{
    return a << s | b >> (8 - s);
}

void read_bits()
{
    FILE *fp;
    int buffer;
    int temp;
    fp = fopen("data-files/comp2.bin", "rb");

    int i, count = 0;
    while ((buffer = getc(fp)) != EOF)
    {
        printf("%02x ", buffer);
        int i;
        for (i = 0; i < 8; i++)
        {
            if (get_bit(buffer, i))
            {
                temp = buffer;
                buffer = getc(fp);
            }
            count++;
        }
    }

    fclose(fp);
}

int main(int argc, char **argv)
{
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

00101000001101000011010100010101010001001010000100000000
2834351544A100
*/