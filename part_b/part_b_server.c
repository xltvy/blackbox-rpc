// Author: Altay Acar
// Author ID: 2018400084

#include "part_b.h"
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 10000
#define READ_END 0
#define WRITE_END 1
#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

char **
partb_1_svc(input *argp, struct svc_req *rqstp)
{
	static char * result;

	int status;	//For the exit status of child process

	pid_t pid;
	char write_buffer[BUFFER_SIZE], read_buffer[BUFFER_SIZE];

	int pipe_P2C[2];	//Parent to child communication pipe
	int pipe_C2P[2];	//Child to parent communication pipe

	if (pipe(pipe_P2C) == -1)	//Checks if the pipe from parent to child is successful
	{
		fprintf(stderr, "ERROR: Failed to create the first pipe!\n");
		return (void *)1;
	}

	if (pipe(pipe_C2P) == -1)	//Checks if the pipe from child to parent is successful
	{
		fprintf(stderr, "ERROR: Failed to create the second pipe!\n");
		return (void *)1;
	}

	pid = fork();	//Child process is created

	if (pid < 0)	//Checks if forking a child process is successful
	{
		fprintf(stderr, "ERROR: Failed to fork a child process!\n");
		exit(-1);
	}

	if (pid == 0)	//Execution of the child process
	{
		dup2(pipe_P2C[READ_END], STDIN_FILENO);	//child's standard input -> read end of forward pipe

		dup2(pipe_C2P[WRITE_END], STDOUT_FILENO);	//child's standard output -> write end of the reverse pipe
		dup2(pipe_C2P[WRITE_END], STDERR_FILENO);	//child's standard error -> write end of the reverse pipe

		//Unnecessary pd's are closed
		close(pipe_C2P[READ_END]);
		close(pipe_C2P[WRITE_END]);
		close(pipe_P2C[READ_END]);
		close(pipe_P2C[WRITE_END]);

		//Child process is turned into blackbox executable
		execl(argp->path, "./blackbox", read_buffer, NULL);
	}
	else	//Execution of the parent process
	{
		close(pipe_P2C[READ_END]);
		close(pipe_C2P[WRITE_END]);

		sprintf(write_buffer, "%d %d\n", argp->a, argp->b);

		write(pipe_P2C[WRITE_END], write_buffer, sizeof(write_buffer));	//Both integers are sent to the child process

		close(pipe_P2C[WRITE_END]);	//Child knows that the message sending process is over

		wait(&status);	//Parent waits for child process to complete its execution
		read(pipe_C2P[READ_END], read_buffer, sizeof(read_buffer));

		char response[BUFFER_SIZE+50];	//Response to be sent to the client

		if (status == 0)	//Successful execution
		{
			sprintf(response, "%s\n%s", "SUCCESS:", read_buffer);
		} else	//Erroneous execution
		{
			sprintf(response, "%s\n%s", "FAIL:", read_buffer);
		}

		result = response;
	}

	memset(&(read_buffer[0]), 0, BUFFER_SIZE);	//Read buffer is reset

	return &result;	//Response is sent to the client
}
