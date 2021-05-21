#include <stdio.h>

#include <stdlib.h>

#include <string.h>

char *compress(char *str)
{
    int a;
    int b = 0;
    int c = 1;
    int size_str = strlen(str);
    char *compress_str = malloc(size_str + 1);
    if (size_str < 2)
    {
        return str;
    }

    for (a = 0; a < size_str; a++)
    {
        if (a == 0)
        {
            compress_str[b] = str[a];
        }
        else
        {
            if (str[a] == str[a - 1])
            {
                compress_str[b] = str[a];
                if (c >= 9 && c < 99)
                {
                    c++;
                    compress_str[b + 1] = (c / 10) + 48;
                    compress_str[b + 2] = (c % 10) + 48;
                }
                else if (c >= 99)
                {
                    c++;
                    compress_str[b + 1] = (c / 100) + 48;
                    compress_str[b + 2] = ((c / 10) % 10) + 48;
                    compress_str[b + 3] = (c % 10) + 48;
                }
                else
                {
                    c++;
                    compress_str[b + 1] = c + 48;
                }
            }
            else
            {
                if (c >= 10 && c < 100)
                {
                    b = b + 3;
                    c = 1;
                    compress_str[b] = str[a];
                }
                else if (c >= 100)
                {
                    b = b + 4;
                    c = 1;
                    compress_str[b] = str[a];
                }
                else if (c > 1 && c <= 9)
                {
                    b = b + 2;
                    c = 1;
                    compress_str[b] = str[a];
                }
                else if (c == 1)
                {
                    b++;
                    compress_str[b] = str[a];
                }
            }
        }
    }
    return compress_str;
}

int main()
{
    printf("type in sample string \n");

    char *res;
    char *str;
    str = (char *)malloc(10240 * sizeof(char));
    scanf("\n%[^\n]", str);
    printf("Decompressed state: \n%s\n", str);

    res = compress(str);
    printf("Compressed state: \n%s\n", res);
    return 0;
}
