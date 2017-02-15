#pragma once

void printPrompt()
{
	char promptString[11] = "CoolShell>";
	printf("%s ", promptString);
}

char * getUserInput()
{
	int n, size = 260;
	char * input = (char*)malloc(size);
	if (!input)				//make sure memory allocation worked
		return NULL;

	do
	{
		printPrompt();			//display prompt
		fgets(input, 256, stdin);	//wait and get user input
		n = strlen(input);
	} while (n <= 1);

	if (input[n - 1] == '\n')		//remove new line from input array
		input[n - 1] = '\0';

	return input;
}

char * parseCommand(char * userInput)
{
	int i, j;
	char * command = (char*) malloc(100);
	int inputLength = strlen(userInput);

	//get string up to first whitespace to separate the command and its parameters
	for (i = 0, j = 0; i < inputLength; i++)
	{
		if (userInput[i] == ' ')
			continue;
		else
		{
			command[j++] = userInput[i];
			if (userInput[i + 1] == ' ' || userInput[i + 1] == '\0')
				break;	//command has been fully stored
		}
	}

	command[j] = '\0';	//null terminate string
	return command;
}


char ** getArgArray(char * input)
{
	int i, row = 0, rows = 2, col = 0, cols = 20, numParams = 0;
	char ** inputArr = (char**)malloc(rows * sizeof(char*));	//initialize 2 rows
	for (i = 0; i < rows; i++)
		inputArr[i] = (char*)malloc(cols * sizeof(char));	//initialize 10 columms per row (will grow if needed)

	int inputLength = strlen(input);

	for (i = 0; i < inputLength; i++)
	{
		if (input[i] == ' ')
			continue;
		else
		{
			if (numParams >= 2)								//dynamically adds one more row if there are 2 or more parameters
			{
				inputArr = (char**)realloc(inputArr, (rows + 1)*sizeof(char*));
				inputArr[rows++] = (char*)malloc(cols * sizeof(char));			//allocate columns in new row, update number of total rows
			}

			inputArr[row][col++] = input[i];		//character is detected, so insert character into 2D array

			if (col == cols - 1)				//if true, reallocate column size on current row because next char won't fit
			{									//in params[row] when including the null character					
				inputArr[row] = (char*)realloc(inputArr[row], 2 * cols * sizeof(char)); //update params[row] with new size of columns
				cols = cols * 2;
			}

			if (input[i + 1] == ' ' || input[i + 1] == '\0')
			{
				inputArr[row][col] = '\0';		//null terminate string since this is the end of the parameter/command read
				row++;					//go to next row since the command or a parameter has been fully stored
				numParams++;				//increment number of parameters
				col = 0;				//reset current column to 0 to iterate through next row
			}
		}
	}

	if (numParams == 1)						//there was only one parameter stored so free the second unused row
		free(inputArr[1]);
	else if (numParams == 0)					//there were no parameters so free space that was allocated
	{
		for (i = 0; i < 2; i++)
			free(inputArr[i]);

		free(inputArr);
	}
	return inputArr;
}

int getNumArgs(char * input)
{
	int i = 0;
	int numArgs = 0;
	int length = strlen(input);

	if (length == 0)
		return 0;

	for (i = 0; i < length; i++)
	{
		if (input[i] == ' ')
			continue;
		else if (input[i + 1] == ' ' || input[i + 1] == '\0')
			numArgs++;
	}
	return numArgs;
}

int parsePath(char ** dirs)	//returns the number of paths parsed
{
	char * pathEnvVar, *thePath;
	int i = 0;

	pathEnvVar = (char *)getenv("PATH");
	thePath = (char *)malloc(strlen(pathEnvVar) + 1);
	strcpy(thePath, pathEnvVar);

	char * token = strtok(thePath, ":");
	while (token != NULL)
	{
		//printf("In parsePath(): %s\n", token);	//for testing only
		strcpy(dirs[i++], token);
		token = strtok(NULL, ":");
	}

	return i;
}

char * lookupPath(char * command, char ** dirs, int numPaths)
{
	char * result;
	char pName[128];
	int i;

	if (*command == '/' || strcmp(command, "./") == 0)	//checks if absolute path was provided as the command
	{
		result = (char *)malloc(strlen(command) + 1);
		strcpy(result, command);
		return result;
	}

	for(i = 0; i < numPaths; i++)
	{
		strcpy(pName, dirs[i]);
		strcat(pName, "/");
		strcat(pName, command);
		//printf("In lookupPath(): %s\n", pName);	//for testing only
		if (access(pName, F_OK | X_OK) == 0)
		{
			result = (char *)malloc(strlen(pName));
			strcpy(result, pName);
			return result;
		}
	}

	return NULL;
}


void exitShell(int numArgs)
{
	if (numArgs == 0)
	{
		printf("Thank you for using the Cool Shell.\nGoodbye.\n\n");
		exit(0);
	}
	else
		printf("ERROR: The 'exit' command does not have any parameters.\n\n");
}
