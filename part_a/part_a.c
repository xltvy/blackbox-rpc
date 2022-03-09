// Author: Altay Acar
// Author ID: 2018400084

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 10000
#define READ_END 0
#define WRITE_END 1
#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char *argv[])
{
	if (argc != 3)	//Checks if the given command in the CLI is correct
	{
		fprintf(stderr, "Usage: %s ./blackbox ./part_a_output.txt\n", argv[0]);
		return 1;
	}

	//Path of the blackbox, given as the first parameter
	char blackbox_path[strlen(argv[1])+1];
	strcpy(blackbox_path, argv[1]);
	//Path of the output file, given as the second parameter
	char output_path[strlen(argv[2])+1];
	strcpy(output_path, argv[2]);

	int o_file;
	o_file = open(output_path, CREATE_FLAGS, CREATE_MODE);

	if (o_file == -1)	//Checks if the given output file can be opened
	{
		fprintf(stderr, "ERROR: Failed to open the file at %s\n", output_path);
		return 1;
	}

	int a;	//First input to blackbox
	int b;	//Second input to blackbox

	scanf("%d %d", &a, &b);

	int status;	//For the exit status of child process

	pid_t pid;
	char write_buffer[BUFFER_SIZE], read_buffer[BUFFER_SIZE];

	int pipe_P2C[2];	//Parent to child communication pipe
	int pipe_C2P[2];	//Child to parent communication pipe

	if (pipe(pipe_P2C) == -1)	//Checks if the pipe from parent to child is successful
	{
		fprintf(stderr, "ERROR: Failed to create the first pipe!\n");
		return 1;
	}

	if (pipe(pipe_C2P) == -1)	//Checks if the pipe from child to parent is successful
	{
		fprintf(stderr, "ERROR: Failed to create the second pipe!\n");
		return 1;
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
		execl(blackbox_path, "./blackbox", read_buffer, NULL);
	}
	else	//Execution of the parent process
	{
		close(pipe_P2C[READ_END]);
		close(pipe_C2P[WRITE_END]);

		sprintf(write_buffer, "%d %d\n", a, b);

		write(pipe_P2C[WRITE_END], write_buffer, sizeof(write_buffer));	//Both integers are sent to the child process

		close(pipe_P2C[WRITE_END]);	//Child knows that the message sending process is over

		wait(&status);	//Parent waits for child process to complete its execution

		memset(&(read_buffer[0]), 0, BUFFER_SIZE);	//Read buffer is reset
		read(pipe_C2P[READ_END], read_buffer, sizeof(read_buffer));

		dup2(o_file, STDOUT_FILENO);	//parent's standard output -> output file
		dup2(o_file, STDERR_FILENO);	//parent's standard error -> output file

		WEXITSTATUS(status);	//Exit status of child process

		if (status == 0)	//Successful execution
		{
			printf("SUCCESS:\n%s", read_buffer);	//Success message printed to the output file
		}
		else	//Erroneous execution
		{
			printf("FAIL:\n%s", read_buffer);	//Fail message printed to the output file
		}
	}

	return 0;
}