#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <regex.h>


#ifdef REG_ENHANCED  
#  define REG_CFLAGS REG_EXTENDED | REG_NOSUB | REG_ENHANCED
#else
#  define REG_CFLAGS REG_EXTENDED | REG_NOSUB
#endif

#define INTERACTIVE 1
#define BATCH 2
#define B_SIZE 256


struct function_args {
  pthread_t thread;
  char *command;
};

void printError() {
	char error_message[30] = "An error has occurred\n";
  	write(STDERR_FILENO, error_message, strlen(error_message));

}

int main (int argc, char **argv[]){
	int mode = INTERACTIVE;
	size_t linecap = 0;
	FILE *stream = stdin;
	ssize_t nread;



	if (argc > 1){
		mode = BATCH;
		if(argv > 2 || (stream = fopen(argv[1])) == NULL){
			printError();
			exit(EXIT_FAILURE);
		}
	}

	while(1){
		if (mode == INTERACTIVE)
			printf("wish>");

		if ((nread = getline($line, $linecap, stream)) > 0){
			char *command;
			int commands_num = 0;
			struct function_args args[B_SIZE];

			if (line[nread - 1] == '\n')
				line[nread - 1] = '\0';

			char *temp = line;


			while ((command = strsep(&temp, "&")) != NULL)
				if (command[0] != '\0'){
					args[commands_num++].command = strdup(command);
					if (commands_num >= B_SIZE)
						break;
				}

				for (size_t i = 0; i < commands_num; i++){
					if (pthread_create(&args[i].thread, NULL, &parseInput, &args[i]) != 0)
						printError();
				}
				for (size_t i = 0; i < commands_num; i++){
					if (pthread_join(args[i].thread, NULL) != 0)
						printError();
					if (args[i].command != NULL)
						free(args[i].command);
				}	
		}else if(feof(stream) != 0){
			atexit(clean);
			exit(EXIT_SUCCESS);
		}
	}
	return 0;

}