#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1000

int main(int argc, char *argv[]){
    
    for (int i = 1; i < argc; i++){
        FILE *file = fopen(argv[i], "r");
        if (file == NULL){
            printf("wcat: cannot open file\n");
            exit(1);
        }

        char  str[MAXLINE];
        while (fgets(str, sizeof str, file) != NULL){
            printf("%s", str);
        }
        fclose(file);
    }
    return 0;
}