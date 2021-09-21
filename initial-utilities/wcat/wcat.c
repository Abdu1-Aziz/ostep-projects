#include <stdio.h>

int main(int argc, char *argv[])
{
    char  str[100];
    FILE *fp;
    fp = fopen(argv[1], "r");
    while (fgets(str, sizeof str, fp) != NULL)
        printf("%s\n", str);
    fclose(fp);
    return 0;
}