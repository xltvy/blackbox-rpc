// Author: Altay Acar
// Author ID: 2018400084

#include "part_b.h"
#include <fcntl.h>
#include <unistd.h>

#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

char output[255];	//Stores the path of the output file

void
partb_prog_1(char *host, char *bb_path, int x, int y)
{
	CLIENT *clnt;
	char * *result_1;
	input  partb_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, PARTB_PROG, PARTB_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	partb_1_arg.path = bb_path;
	partb_1_arg.a = x;
	partb_1_arg.b = y;
	result_1 = partb_1(&partb_1_arg, clnt);
	if (result_1 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	} else	//Program works as expected
	{
		int o_file;
		o_file = open(output, CREATE_FLAGS, CREATE_MODE);

		if (o_file == -1)	//Checks if the given output file can be opened
		{
			fprintf(stderr, "ERROR: Failed to open the file at %s\n", output);
			return;
		}

		dup2(o_file, STDOUT_FILENO);	//this process' standard output -> output file

		printf("%s", *result_1);
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 4) {
		printf ("Usage: %s ./blackbox ./part_b_output.txt server_host\n", argv[0]);
		exit (1);
	}

	//Path of the blackbox, given as the first parameter
	char blackbox_path[strlen(argv[1])+1];
	strcpy(blackbox_path, argv[1]);
	//Path of the output file, given as the second parameter
	char output_path[strlen(argv[2])+1];
	strcpy(output_path, argv[2]);
	strcpy(output, output_path);

	host = argv[3];

	int a;	//First input to blackbox
	int b;	//Second input to blackbox

	scanf("%d %d", &a, &b);

	partb_prog_1 (host, blackbox_path, a, b);
exit (0);
}
