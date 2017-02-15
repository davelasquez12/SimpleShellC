/*David Velasquez
Operating Systems - Proj1 - Simple Shell
Due: February 14th, 2017*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "functions.h"

int main()
{
	int i = 0, j, k, numArgs = 0;
	char ** argArray, ** argArrayFree = NULL;
	printf("\nWelcome to the Cool Shell\n\n");

	char ** pathv = (char**)malloc(64 * sizeof(char*));	//init number of paths (64)
	for (i = 0; i < 64; i++)				//init the path length of each path in the array (128 chars)
	{
		pathv[i] = (char*)malloc(128);	
	}				

	int numPaths = parsePath(pathv);

	while (1)
	{
		char * input = getUserInput();			//prompt function for user input
		char * savedInput = input;			//free this memory allocated at the end
		char * command;					//stores only the command (not the arguments)

		command = parseCommand(input);
		char * fullCmdPath = lookupPath(command, pathv, numPaths);	//needed once path searching works

		numArgs = getNumArgs(input) - 1;	//minus 1 to not include the command in the count

		argArray = getArgArray(input);
		argArrayFree = argArray;
	
		if (strcmp(command, "exit") == 0)
			exitShell(numArgs);

		//start child process
		if (fork() == 0)
		{
			execv(fullCmdPath, argArray);
			exit(0);	//should not reach here
		}

		//parent process
		wait(NULL);

		free(command);
		free(savedInput);
		free(fullCmdPath);
		for (i = 0; i < numArgs + 1; i++)
			free(argArrayFree[i]);
	}
	

	for (i = 0; i < 64; i++)			
		free(pathv[i]);		

	free(pathv);

	return 0;
}
