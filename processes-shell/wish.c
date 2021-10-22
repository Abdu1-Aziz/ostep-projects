#include <pthread.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/wait.h> 


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

void print_error() {
	char error_message[30] = "An error has occurred\n";
  	write(STDERR_FILENO, error_message, strlen(error_message));

}

void *parse_input(void *arg);
int search_path(char path[], char *firstArg);
void redirect(FILE *out);
void execute_commands(char *args[], int args_num, FILE *out);
char *trim(char *);
void clean(void);

FILE *stream = NULL;
char *paths[B_SIZE] = {"/bin", NULL};
char *line = NULL;


void clean(void) {
  free(line);
  fclose(stream);
}


char *trim(char *s) {
  while (isspace(*s))
    s++;

  if (*s == '\0')
    return s;

  char *end = s + strlen(s) - 1;
  while (end > s && isspace(*end))
    end--;

  end[1] = '\0';
  return s;
}


void *parse_input(void *arg) {
  char *args[B_SIZE];
  int args_num = 0;
  FILE *output = stdout;
  struct function_args *fun_args = (struct function_args *)arg;
  char *commandLine = fun_args->command;

  char *command = strsep(&commandLine, ">");
  if (command == NULL || *command == '\0') {
    print_error();
    return NULL;
  }

  command = trim(command);

  if (commandLine != NULL) {
    // contain white space in the middle or ">"
    regex_t reg;
    if (regcomp(&reg, "\\S\\s+\\S", REG_CFLAGS) != 0) {
      print_error();
      regfree(&reg);
      return NULL;
    }
    if (regexec(&reg, commandLine, 0, NULL, 0) == 0 ||
        strstr(commandLine, ">") != NULL) {
      print_error();
      regfree(&reg);
      return NULL;
    }

    regfree(&reg);

    if ((output = fopen(trim(commandLine), "w")) == NULL) {
      print_error();
      return NULL;
    }
  }

  char **ap = args;
  while ((*ap = strsep(&command, " \t")) != NULL)
    if (**ap != '\0') {
      *ap = trim(*ap);
      ap++;
      if (++args_num >= B_SIZE)
        break;
    }

  if (args_num > 0)
    execute_commands(args, args_num, output);

  return NULL;
}

int search_path(char path[], char *firstArg) {
  // search executable file in path
  int i = 0;
  while (paths[i] != NULL) {
    snprintf(path, B_SIZE, "%s/%s", paths[i], firstArg);
    if (access(path, X_OK) == 0)
      return 0;
    i++;
  }
  return -1;
}

void redirect(FILE *out) {
  int outFileno;
  if ((outFileno = fileno(out)) == -1) {
    print_error();
    return;
  }

  if (outFileno != STDOUT_FILENO) {
    // redirect output
    if (dup2(outFileno, STDOUT_FILENO) == -1) {
      print_error();
      return;
    }
    if (dup2(outFileno, STDERR_FILENO) == -1) {
      print_error();
      return;
    }
    fclose(out);
  }
}

void execute_commands(char *args[], int args_num, FILE *out) {
  // check built-in commands first
  if (strcmp(args[0], "exit") == 0) {
    if (args_num > 1)
      print_error();
    else {
      atexit(clean);
      exit(1);
    }
  } else if (strcmp(args[0], "cd") == 0) {
    if (args_num == 1 || args_num > 2)
      print_error();
    else if (chdir(args[1]) == -1)
      print_error();
  } else if (strcmp(args[0], "path") == 0) {
    size_t i = 0;
    paths[0] = NULL;
    for (; i < args_num - 1; i++)
      paths[i] = strdup(args[i + 1]);

    paths[i + 1] = NULL;
  } else {
    // not built-in commands
    char path[B_SIZE];
    if (search_path(path, args[0]) == 0) {
      pid_t pid = fork();
      if (pid == -1)
        print_error();
      else if (pid == 0) {
        // child process
        redirect(out);

        if (execv(path, args) == -1)
          print_error();
      } else
        waitpid(pid, NULL, 0); // parent process waits child
    } else
      print_error(); // not fond in path
  }
}

int main (int argc, char **argv){
	int mode = INTERACTIVE;
	size_t linecap = 0;
	stream = stdin;
  char *line;
	ssize_t nread;

	if (argc > 1){
		mode = BATCH;
  	if(argc > 2 || (stream = fopen(argv[1], "r")) == NULL){
  		print_error();
  		exit(0);
  	}
  }
	while(1){
		if (mode == INTERACTIVE)
		printf("wish>");

		if ((nread = getline(&line, &linecap, stream)) > 0){
			char *command;
			int commands_num = 0;
			struct function_args args[B_SIZE];

			if (line[nread - 1] == '\n')
				line[nread - 1] = '\0';

			char *temp = line;


			while ((command = strsep(&temp, "&")) != NULL){
				if (command[0] != '\0'){
					args[commands_num++].command = strdup(command);
					if (commands_num >= B_SIZE)
						break;
				}
      }
			for (size_t i = 0; i < commands_num; i++){
				if (pthread_create(&args[i].thread, NULL, &parse_input, &args[i]) != 0)
					print_error();
			}
			for (size_t i = 0; i < commands_num; i++){
				if (pthread_join(args[i].thread, NULL) != 0)
					print_error();
				if (args[i].command != NULL)
					free(args[i].command);
			}
				
		}else if(feof(stream) != 0){
      //print_error();
			atexit(clean);
			exit(1);
		}
	}
	return 0;

}