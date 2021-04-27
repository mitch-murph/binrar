#include "vector.h"
#include <stdio.h>
#define UNIT long long int

void func(int pos, void *value)
{
    UNIT *c = (UNIT *)(value);
    printf("[%d] %lld\n", pos, *c);
}

int main(int argc, char **argv)
{
    vector_t v;
    init_vector(&v, 10, sizeof(UNIT));
    UNIT i;
    for (i = 0; i <= 25; i++)
    {
        vector_push_back(&v, (void *)&i);
    }
    print_vector(v, func);
    free_vector(v);
    return 0;
}