#include <stdio.h> /* printf, fputc, fgetc */
#include "bit_array.h"

int get_bit(int v, int i)
{
    return (v & (1 << (i))) != 0;
}

int combine_bits(int a, int b, int s)
{
    return a << s | b >> (8 - s);
}

void print_bits(int c)
{
    int i;
    for (i = 31; i >= 0; i--)
    {
        if ((i + 1) % 8 == 0)
            printf(" ");
        printf("%d", get_bit(c, i));
    }
}

void print_bits_length(int c, int l)
{
    int i;
    for (i = l; i >= 0; i--)
        printf("%d", get_bit(c, i));
}

int convert_vector_to_bit_array(vector_t vector)
{
    int i, bits = 0;
    for (i = 0; i < vector.size; i++)
    {
        int x = *(int *)vectorGet(vector, i);
        bits = (bits << 1) + x;
    }
    return bits;
}

void write_bit(int *buffer, int *buffer_size, int value, int size, FILE *fp)
{
    if (size == -1)
    {
        value = 0;
        size = (8 - *buffer_size);
    }

    int i;
    for (i = size - 1; i >= 0; i--)
    {
        *buffer = (*buffer << 1) + get_bit(value, i);
        (*buffer_size)++;
        if (*buffer_size == 8)
        {
            fputc(*buffer, fp);
            *buffer = 0;
            *buffer_size = 0;
        }
    }
}

int read_bit(int *buffer, int *buffer_size, FILE *fp)
{

    if (*buffer_size == 0)
    {
        *buffer = fgetc(fp);
        *buffer_size = 7;
    }
    else
    {
        (*buffer_size)--;
    }
    return get_bit(*buffer, *buffer_size);
}

int read_n_bit(int *buffer, int *buffer_size, int n, FILE *fp)
{
    int temp = 0;
    while (n--){
        int a = read_bit(buffer, buffer_size, fp);
        temp = (temp << 1) + a;
    }
    return temp;
}