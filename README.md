# blackbox-rpc

Bogazici University

Computer Engineering Department

Fall 2021

CMPE 322 - Operating Systems

Project 1

Altay Acar

***

An unknown binary file named blackbox is operated to take some input from the console and it displays some output based on the given input values using RPC as the layer of communication between the client and server.

The project consists of two separate parts; local procedure and remote procedure. The local procedure part includes the creation of several processes and executing a binary file. The remote procedure part includes the creation of remote processes and executing a binary file.

The project focuses on an unknown binary file, namely blackbox. The blackbox is an executable file that we use without knowing the source code. What we know about the blackbox is that it takes some input from the console (i.e., by reading stdin, not command line parameters) and it displays some output based on the given input values.

In Part A, we run the blackbox with the given inputs, get the output from the blackbox and redirect it as requested.

In Part B, we upgrade the wrapper designed in Part A to be a Remote Procedural Call (RPC) server that executes the blackbox for any client that requests.

Therefore there are two different codes:

Part A:
- A makefile, the source code written in C and the executable .out file are provided in part_a folder.
- The main program creates a child process that executes the given blackbox.
- The parent process reads the input from stdin and passes it to the blackbox as input. The blackbox takes two integer values from stdin as input and either produces one integer to stdout or an error text to stderr as output.
- Main program captures the output of the blackbox and prints it to the specified file in the following format:
  > input1 input2 output1

Part B:
- A makefile, the source code written in C, and automatically generated source and executable files are provided in part_b folder.
- Client program calls an RPC function in the server, passing three parameters: path, number1, and number2
- Server program is ready for clients to run the RPC function given above. This function creates a child process that executes the given blackbox and return the response to the client.
- The client program prints the response to the specified file in the following format:
  > input1 input2 output1

Testing:
- Part A is tested using these commands to the CLI:
  > make
  > 
  > ./part_a.out blackbox part_a_output.txt
  > 
- Part B is tested using these commands to the CLI (server and client programs are run in different terminals):
  > make
  > 
  > ./part_b_server.out &
  > 
  > ./part_b_client.out blackbox part_b_output.txt SERVER_IP_ADDRESS

Input:
- For Part A:
  > The part a.out program takes two command line arguments:
  > 
  > blackbox: Path of the executable file
  > 
  > part a output.txt: Path of the output file
- For Part B:
  > The part b server.out program takes no argument and runs in the background.
  > 
  > The part b client.out program takes three command line arguments:
  > 
  > blackbox: Path of the executable file
  > 
  > part b output.txt: Path of the output file
  > 
  > SERVER IP ADDRESS: IP address of the server

Output:
- The blackbox returns an integer if the process executes successfully, and an error message if the process fails for some reason. Program captures the output and write SUCCESS: or FAIL: to the output file accordingly.
