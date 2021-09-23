#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 1000


int getLength (char* s){
    int r = 0;
    while(s[r] != '\n' && s[r] != '\0'){
        r++;
    }
    return r;
}

int main(int argc, char *argv[]){

    if (argc < 2){
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    }
    else if (argc == 2){
        char *pattern = argv[1];
        int patternLength = getLength(pattern);

        char *line = NULL;
        size_t size = 0;

        line = (char*)malloc(size);

        while(getline(&line, &size, stdin) >= 0) {
		    int r = getLength(line);
            int i = 0;
            while(line[i] != '\0' && line[i] != '\n'){
                if (line[i] == pattern[0]){
                    int found = 1;
                    for (int j = 0; j < patternLength; j++){
                        if (i+j >= r){
                            found = 0;
                            break;
                        }
                        if (pattern[j] != line[i+j]){
                            found = 0;
                            break;
                        }
                    }
                    if (found){
                        printf("%s", line);
                    }
                }
                i++;
            }
	    }
    }
    else if (argc > 2){
        int fileCount = argc - 2;

        char *pattern = argv[1];
        int patternLength = getLength(pattern);
        for (int k = 0; k < fileCount; k++){
            FILE *file = fopen(argv[k+2], "r");

            if (file == NULL){
                printf("wgrep: cannot open file\n");
                exit(1);
            }

            char *line = NULL;
            size_t size = 10000;

            line = (char*)malloc(size);
            int count = 0;

            while (getline(&line, &size, file) >= 0){
                int r = getLength(line);
                int i = 0;

                while (line[i] != '\0' && line[i] != '\n'){
                    if (line[i] == pattern[0]){
                        int found = 1;
                        for (int j = 0; j < patternLength; j++){
                            if (i+j >= r){
                                found = 0;
                                break;
                            }
                            if (pattern[j] != line[i+j]){
                                found = 0;
                                break;
                            }
                        }
                        if (found){
                            printf("%s", line);
                            break;
                        }
                    }
                    i++;
                }
                count++;
            }
        }
    }
    return 0;
}