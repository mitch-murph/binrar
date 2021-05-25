#include "src/teacher_interface.h"
#include "src/compress.h"
#include <stdio.h>

void compress_test()
{
    FILE *in_fp = fopen("koala.bmp", "rb+");
    printf("%p\n", in_fp);
    FILE *out_fp = fopen("compressed.bin", "wb+");
    printf("%p\n", out_fp);
    huffmanCompress(in_fp, out_fp);
    fclose(in_fp);
    fclose(out_fp);

    printf("done\n");

    in_fp = fopen("compressed.bin", "rb+");
    printf("%p\n", in_fp);
    out_fp = fopen("dfg.bmp", "wb+");
    printf("%p\n", out_fp);
    huffmanDecompress(in_fp, out_fp);
    fclose(in_fp);
    fclose(out_fp);
}

int main(void)
{
    teacherMainMenu();
    return 0;
}
