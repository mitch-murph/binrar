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
    /*vector_t fns;
    init_vector(&fns, 1, sizeof(char **) * 255);
    char *s;
    s = "data-files/comp.bin";
    vector_push_back(&fns, s);
    s = "data-files/compp.bin";
    vector_push_back(&fns, s);

    combine_files(fns, "data-files/out.bin");
    separate_files("data-files/out.bin");

    FILE*fp = fopen("data-files/out.bin", "rb");
    seek_file_contents(fp);*/

    /*
    int buffer = 0;
    int buffer_size = 0;
    int value = 3;
    int size = 3;
    write_bit(&buffer, &buffer_size, value, size);
    value = 3;
    size = 3;
    write_bit(&buffer, &buffer_size, value, size);
    value = 0;
    size = 4;
    write_bit(&buffer, &buffer_size, value, size);
    value = 2;
    size = 3;
    write_bit(&buffer, &buffer_size, value, size);
    write_bit(&buffer, &buffer_size, 0, -1);
    */
    

   
    /*printf("%02x\n", buffer);*/

    compress("data-files/comp.bin", "data-files/out.bin");

    /*
    read_bits();

    fp = fopen("data-files/two.bin", "rb");
    char c;
    int count = 0;
    while ((c = getc(fp)) != EOF || count < 2)
    {
        if (c == EOF)
            count++;
        printf("%c\n", c);
    }
    fclose(fp);

    compress();
    read_bits();
    int buffer = 0;
    int count = 0;
    buffer = (buffer << 1);
    count++;
    buffer = (buffer << 1);
    count++;
    buffer = (buffer << 1) + 1;
    count++;


    int key = 'A';
    int a = key >> count;
    buffer = buffer << 8 - count;
    int temp = key << 8 - count;
    buffer = buffer | a;

    print_bits(buffer);
    print_bits(key);
    print_bits(a);
    print_bits(temp);
    print_bits(buffer);*/

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

001 01000001 1 01000011 01 01000101 01 01000100 1 0100001 00000000
00101000 00110100  00110101  00010101  01000100 10100001  00000000
2834351544A100
*/

/*

01 01000011 01 01000010 1 01000001 000
C 01000011
B 01000010
A 01000001





01010000110101000010101000001000

00000000 -> 43
00000010 -> 42
00000011 -> 41

[43] 01000011
[42] 01000010
[41] 01000001



*/