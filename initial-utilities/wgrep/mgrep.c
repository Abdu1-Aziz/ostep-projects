#include<stdio.h>
#include<stdlib.h>
#include<string.h>





//Our main function.
int main(int argc, char *argv[]) {
	int errno = 0;

	 
	if(Search_in_File(argv[1], argv[2]) == -1) {
		perror("Error");
		printf("Error number = %d\n", errno);
		exit(1);
	}
	return(0);
}


int Search_in_File(char *fname, char *str) {
	FILE *fp;
	int find_result = 0;
	char temp[512];
	
	//gcc users
	if((fp = fopen(fname, "r")) == NULL) {
		return(-1);
	}

	while(fgets(temp, 512, fp) != NULL) {
		if((strstr(temp, str)) != NULL) {
			printf("\n%s\n", temp);
			find_result++;
		}
	}

	if(find_result == 0) {
		printf("\nSorry, couldn't find a match.\n");
	}
	
	//Close the file if still open.
	if(fp) {
		fclose(fp);
	}
   	return(0);
}