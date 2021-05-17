#include "bit_array.h"
#include <stdio.h>

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
        int x = *(int *)vector_get(vector, i);
        bits = (bits << 1) + x;
    }
    return bits;
}