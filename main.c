#include "compress.h"
#include <stdio.h>

void generate_bin(){
    FILE *fp;
    fp = fopen("data-files/comp.bin", "wb");
    int a[] = {'A', 'B', 'A', 'B', 'C', 'A', 'A', 'D', 0xffff};
    int i;
    for (i = 0; a[i] != 0xffff; i++){
        fwrite(&a[i], sizeof(char), 1, fp);
    }
    fclose(fp);
}

int main(int argc, char **argv)
{
    compress();
    return 0;
}