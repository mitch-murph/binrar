#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include "vector.h"

int get_bit(int v, int i);
int combine_bits(int a, int b, int s);
void print_bits(int c);
void print_bits_length(int c, int l);
int convert_vector_to_bit_array(vector_t vector);

#endif