#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]){

    char line[1024];
    //char *temp = NULL;
    char *tofind = NULL;
    tofind = argv[1];

    FILE *fp = fopen(argv[2], "r");
    if (fp == NULL)
    {
        printf("cannot open file \n");
        exit(1);
    }

    while(fgets(line, 512, fp) != NULL) {
		if((strstr(line, tofind)) != NULL) {
			printf("\n%s\n", line);
		}
	}

    fclose(fp);
}