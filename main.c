#include "vector.h"

int main(int argc, char** argv)
{
    vector_t* v = new_vector(10);
    int i = 10;
    void* ptr = (void*)(&i);
    vector_insert(v, ptr);
    print_vector(v);
    return 0;
}