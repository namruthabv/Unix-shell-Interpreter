#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>

pid_t proc_pid;
static char* arg1[400];
static int no_cmd = 0;
static char buffer1[1000];

#define RETURN_FAIL    0


static int execute_pipe(int val1, int val2, int val3)
{
	int fd[2];

	/* Invoking pipe and error checking */
	if(pipe(fd) < 0 ){
		perror(" ERROR *** PIPE INVOKING ERROR");
		exit(1);
	}

/* Invoking child and error checking */
	if((proc_pid = fork()) < 0){
		perror(" ERROR *** CHILD INVOKING ERROR");
		exit(1);
	}

/* child process executing commands */
	if (proc_pid == 0) {
		/* First command redirection */
		if (val2 == 1 && val3 == 0 && val1 == 0) {
			dup2( fd[1], 1);
		}
		/* Second command redirection */
		else if (val2 == 0 && val3 == 0 && val1 != 0) {
			dup2(val1, 0);
			dup2(fd[1], 1);

		}
		/* Third command redirection */
		 else {
			dup2( val1, 0);

		}

		/* Command execution */
		if (execvp( arg1[0], arg1) == -1)
			exit(1);
	}

	// Closing val1 that is lowest file descriptor
	if (val1 != 0)
		close(val1);

	// Closing write file descriptor.
	close(fd[1]);

	// Closing read file descriptor
	if (val3 == 1)
		close(fd[0]);

		/* Parent waiting for child commands to execute in sequence*/
		int z;
		for (z = 0; z < no_cmd; ++z)
			wait(NULL);

	return fd[0];
}

/* Splitting the commands based on space in order to separate command from options */
static void tokenize(char* cmdLine)
{
	while (isspace(*cmdLine)) ++cmdLine;

  // spliting on occurence of space
	char* pipe_pos = strchr(cmdLine, ' ');
	int i = 0;
	while(pipe_pos != NULL) {
		pipe_pos[0] = '\0';
		arg1[i] = cmdLine;
		++i;
		char* new_pipe_pos = pipe_pos + 1;
		while (isspace(*new_pipe_pos)) ++new_pipe_pos;
		cmdLine = new_pipe_pos;
		pipe_pos = strchr(cmdLine, ' ');
	}

	if (cmdLine[0] != '\0') {
		arg1[i] = cmdLine;
		pipe_pos = strchr(cmdLine, '\n');
		pipe_pos[0] = '\0';
		++i;
	}

	arg1[i] = NULL;
}

/* To run each command by calling execute_pipe method */
static int execute1(char* cmdLine, int val1, int val2, int val3)
{
	tokenize(cmdLine);
	if (arg1[0] != NULL) {
		if (strcmp(arg1[0], "exit") == 0)
			exit(0);
		no_cmd += 1;
		return execute_pipe(val1, val2, val3);
	}
	return 0;
}

int main()
{
	printf("Executing own shell interpreter. \nThis takes upto 3 pipe connected commands.\nPlease provide space before and after pipe. \n");
	while (1) {
		/* Print the command prompt */
		printf("$Namrutha --> ");
		fflush(NULL);

		/* fetching the command */
		if (!fgets(buffer1, sizeof(buffer1), stdin))
			return RETURN_FAIL;

		int val1 = 0, val2 = 1, val3 = 0;

		char* cmdLine = buffer1;
		char* pipe_pos = strchr(cmdLine, '|');

		/* to split the command based on ''|'' */
		while (pipe_pos != NULL) {
			*pipe_pos = '\0';
			val1 = execute1(cmdLine, val1, val2, val3);

			cmdLine = pipe_pos + 1;
			pipe_pos = strchr(cmdLine, '|');
			val2 = 0;
		}
		val3 = 1;
		val1 = execute1(cmdLine, val1, val2, val3);

		no_cmd = 0;
	}
	return 0;
}
