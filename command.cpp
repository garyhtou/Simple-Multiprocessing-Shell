#include "command.h"
#include "helper.h"

#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

string rawCommand;
int *inPipe;
int *outPipe;

Command::Command(string rawCommand)
{
	this->rawCommand = rawCommand;

	// Default to no pipes (for single commands)
	this->inPipe = NULL;
	this->outPipe = NULL;
}
Command::Command(string rawCommand, int *inPipe, int *outPipe)
{
	this->rawCommand = rawCommand;
	this->inPipe = inPipe;
	this->outPipe = outPipe;
}
void Command::run()
{
	// Get the command's arguments by lexing the raw command with the delimiter
	vector<string> args = Helper::lex(this->rawCommand, DELIMITER);

	try
	{
		// Execute the command!!
		execute(args);
	}
	catch (...)
	{
		cout << "Error: Command failed to run" << endl;
	}
}

void Command::execute(vector<string> args)
{
	// Fork the current process
	pid_t pid = fork();

	if (pid == 0) // CHILD PROCESS
	{
		this->childExecute(args);
		// The child will exit in the function. It will never return here.
	}

	// PARENT PROCESS
	// Handle failed fork
	if (pid < 0)
	{
		cout << "Error: Fork failed" << endl;

		// Critical error. Exit program on fork failure
		exit(1);
	}

	// Close pipes
	if (this->inPipe != NULL)
	{
		close(this->inPipe[0]);
		close(this->inPipe[1]);
	}

	// Don't wait for child process to exit here. Only wait for child processes
	// after ALL commands have been ran (forked and executed).
}

void Command::childExecute(vector<string> args)
{
	// Convert the C++ vector of strings to C array
	char *const *argv = Command::stringVectorToCharArray(args);

	// If this command requires an input pipe, then set it up
	if (this->inPipe)
	{
		setInPipe(this->inPipe);
	}

	// If this command requires an output pipe, then set it up
	if (this->outPipe)
	{
		setOutPipe(this->outPipe);
	}

	// Execute the command!
	execvp(argv[0], argv);

	// A successful execvp call will NOT return. This following code will only run
	// if an error with execvp occurs.
	// Errors from the command executed are handled after waitpid.

	// Deallocate argv (necessary if execvp fails)
	for (size_t i = 0; i < args.size(); i++)
	{
		delete[] argv[i];
	}
	delete[] argv;

	perror("Command failed to run");
	exit(1);
}

void Command::setInPipe(int *input)
{
	// Duplicate the file descriptor and close the previous file descriptor
	if (int(dup2(input[0], STDIN_FILENO)) == -1)
	{
		cout << "Error: dup2 failed" << endl;
	};
	if (int(close(input[0])) == -1)
	{
		cout << "Error: close system call failed" << endl;
	};
	if (int(close(input[1])) == -1)
	{
		cout << "Error: close system call failed" << endl;
	};
}

void Command::setOutPipe(int *output)
{
	// Duplicate the file descriptor and close the previous file descriptor
	if (int(dup2(output[1], STDOUT_FILENO)) == -1)
	{
		cout << "Error: dup2 failed" << endl;
	};
	if (int(close(output[0])) == -1)
	{
		cout << "Error: close system call failed" << endl;
	};
	if (int(close(output[1])) == -1)
	{
		cout << "Error: close system call failed" << endl;
	};
}

char *const *Command::stringVectorToCharArray(vector<string> toConvert)
{
	// Create an array of C strings
	// Alot one extra element for the null terminator
	char **charArr = new char *[toConvert.size() + 1];

	// Convert each C++ String to a C string and store in the array
	for (size_t i = 0; i < toConvert.size(); i++)
	{
		charArr[i] = new char[toConvert[i].size() + 1]; // make it fit
		strcpy(charArr[i], toConvert[i].c_str());				// copy string
	}

	// NULL terminate the array
	charArr[toConvert.size()] = (char *)NULL;

	return charArr;
}