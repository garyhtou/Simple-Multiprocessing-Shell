//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
using namespace std;

string rawCommand;

Command::Command(string rawCommand)
{
	printf("DEBUG: Command::Command");
	this->rawCommand = rawCommand;
	printf("DEBUG: rawCommand = %s\n", rawCommand.c_str());
}
Command::~Command()
{
	printf("DEBUG: Command::~Command\n");
	//TODO: might not be needed??
}
void Command::run()
{
	printf("DEBUG: Command::run\n");
	vector<string> args = parse(this->rawCommand);

	try
	{
		execute(args);
	}
	catch (...)
	{
		// first set output code and stdout;
		// handleError(code, errorMessage);
	}
}

vector<string> Command::parse(string rawCommand)
{
	printf("DEBUG: Command::parse\n");
	printf("DEBUG: rawCommand = %s\n", rawCommand.c_str());
	vector<string> parsedArgs;

	// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c

	size_t index = 0;
	string token;
	while ((index = rawCommand.find(this->DELIMITER)) != string::npos)
	{
		token = rawCommand.substr(0, index);
		parsedArgs.push_back(token);
		printf("DEBUG: token = %s\n", token.c_str());
		rawCommand.erase(0, index + 1);
	}
	parsedArgs.push_back(rawCommand);
	printf("DEBUG: token = %s\n", rawCommand.c_str());

	return parsedArgs;
}

void Command::execute(vector<string> args)
{
	printf("DEBUG: Command::execute\n");
	// fork and run
	pid_t pid = fork();

	//handle failed fork
	if (pid < 0)
	{
		cout << "Fork failed" << endl;
	}
	else if (pid == 0) // if child excecute
	{
		this->childExecute(args);
		// The child will exit in the function. It will not return here.
	}

	// parent code
	int status;
	waitpid(pid, &status, 0);
	// TODO: handle error

	cout << "process " << pid << " exits with " << status << endl;
}

void Command::childExecute(vector<string> args)
{
	printf("DEBUG: Command::childExecute\n");
	// convert C++ vector of strings to C array
	char *const *argv = this->argChars(args);

	int code = execvp(argv[0], argv);

	// This code will only run if execvp itself fails.
	// Errors from the command executed is handled after waitpid
	handleError(code, "execvp failed");
}

void Command::handleError(int code, string errorMessage)
{
	printf("DEBUG: Command::handleError\n");
	perror("execvp failed");
	exit(1);
	// https://www.cplusplus.com/reference/cstdio/perror/
}

char *const *Command::argChars(vector<string> toConvert)
{
	char **cc = new char *[toConvert.size() + 1];

	for (int i = 0; i < toConvert.size(); ++i)
	{
		cc[i] = new char[toConvert[i].size() + 1]; //make it fit
		strcpy(cc[i], toConvert[i].c_str());			 //copy string
		printf("DEBUG: cc[%d] = %s\n", i, cc[i]);
	}
	cc[toConvert.size() + 1] = NULL;

	return cc; //pointers to the strings will be const to whoever receives this data.
}
