//
#include "command.h"
#include "helper.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
	vector<string> args = tokenize(this->rawCommand);

	try
	{
		execute(args);
	}
	catch (...)
	{
		printf("\n\nDEBUG: Command::run caught unexpected exception !!!!!!\n\n\n");
	}
}

vector<string> Command::tokenize(string rawCommand)
{
	Helper::debugPrint("Command::tokenize");
	printf("\tDEBUG: rawCommand = %s\n", rawCommand.c_str());
	rawCommand = Helper::trimStr(rawCommand);
	vector<string> tokens;

	// TODO: parse needs to account for strings as a single token
	size_t index = 0;
	string currToken;
	// Split using the delimiter
	while ((index = rawCommand.find(this->DELIMITER)) != string::npos)
	{
		currToken = rawCommand.substr(0, index);
		tokens.push_back(Helper::trimStr(currToken));
		printf("\tDEBUG: currToken = %s\n", Helper::trimStr(currToken).c_str());
		rawCommand.erase(0, index + 1);
	}
	// push the last token
	tokens.push_back(Helper::trimStr(rawCommand));
	printf("\tDEBUG: currToken = %s\n", Helper::trimStr(rawCommand).c_str());

	return tokens;
}

void Command::execute(vector<string> args)
{
	// fork and run
	pid_t pid = fork();

	//handle failed fork
	if (pid < 0)
	{
		// TODO: make sure this follows instructions
		cout << "Fork failed" << endl;
	}
	else if (pid == 0) // if child excecute
	{
		this->childExecute(args);
		// The child will exit in the function. It will never return here.
	}

	// Close pipes before waiting for child to exit
	if (this->inPipe != NULL)
	{
		close(this->inPipe[0]);
		close(this->inPipe[1]);
	}

	int status;
	waitpid(pid, &status, 0);

	if (WIFEXITED(status))
	{
		int code = WEXITSTATUS(status);
		cout << "process " << pid << " exits with " << code << endl;
	}
	else if (WIFSIGNALED(status))
	{
		int signal = WTERMSIG(status);
		// The process was terminated by a signal.
		//TODO: This isn't really required by the instructions. double check
		cout << "process " << pid << " was terminated with " << signal << endl;
	}
	else
	{
		cout << "process " << pid << " exists with "
				 << "unknown code" << endl;
	}
}

void Command::childExecute(vector<string> args)
{
	Helper::debugPrint("Command::childExecute");

	// convert C++ vector of strings to C array
	char *const *argv = Command::stringVectorToCharArray(args);

	if (this->inPipe)
	{
		setInPipe(this->inPipe);
	}

	if (this->outPipe)
	{
		setOutPipe(this->outPipe);
	}

	execvp(argv[0], argv);
	// A successful execvp call will NOT return. This following code will only run
	// if an error with execvp occurs.
	// Errors from the command executed is handled after waitpid.

	// deallocate argv (necessary if execvp fails)
	for (int i = 0; i < args.size(); i++)
	{
		delete[] argv[i];
	}
	delete[] argv;

	perror("Command failed to run");
	exit(1);
}

void Command::setInPipe(int *input)
{
	dup2(input[0], STDIN_FILENO);
	close(input[0]);
	close(input[1]);
}

void Command::setOutPipe(int *output)
{
	dup2(output[1], STDOUT_FILENO);
	close(output[0]);
	close(output[1]);
}

char *const *Command::stringVectorToCharArray(vector<string> toConvert)
{
	char **charArr = new char *[toConvert.size() + 1];

	for (int i = 0; i < toConvert.size(); i++)
	{
		charArr[i] = new char[toConvert[i].size() + 1]; //make it fit
		strcpy(charArr[i], toConvert[i].c_str());				//copy string
		printf("\tDEBUG: charArr[%d] = %s\n", i, charArr[i]);
	}
	charArr[toConvert.size()] = (char *)NULL;

	return charArr; //pointers to the strings will be const to whoever receives this data.
}