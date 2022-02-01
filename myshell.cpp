#include "command.h"
#include "helper.h"

#include <unistd.h>
#include <iostream>

using namespace std;

const char PIPE_DELIMITER = '|';

void runCommands(vector<string> rawCommands)
{
	Helper::debugPrint("runCommands");

	if (rawCommands.size() == 1)
	{
		Command command = Command(rawCommands[0]);
		command.run();
		return;
	}

	vector<Command> commands;

	int inPipe[2];
	int outPipe[2];

	// Set up and run the first command
	pipe(outPipe);

	Command leadingCommand = Command(rawCommands[0], NULL, outPipe);
	commands.push_back(leadingCommand);
	printf("\nRUNNING COMMAND\n\n");
	leadingCommand.run();

	inPipe[0] = outPipe[0];
	inPipe[1] = outPipe[1];

	// Start with the 2nd command and end with the 2nd to last command since the
	// first and last commands use default STDIN/STDOUT
	for (int i = 1; i < rawCommands.size() - 1; i++)
	{
		pipe(outPipe);
		// Create a new command
		Command command = Command(rawCommands[i], inPipe, outPipe);
		commands.push_back(command);
		printf("\nRUNNING COMMAND\n\n");
		command.run();

		inPipe[0] = outPipe[0];
		inPipe[1] = outPipe[1];
	}

	// Set up and run the last command
	Command trailingCommand = Command(rawCommands[rawCommands.size() - 1], inPipe, NULL);
	commands.push_back(trailingCommand);
	printf("\nRUNNING COMMAND\n");
	trailingCommand.run();
}

int main(int argc, char *argv[])
{
	// Display prompt and wait for input
	printf("myshell$");

	// Get multi-command from standard input
	string rawMultiCommand;
	getline(cin, rawMultiCommand);

	printf("\tDEBUG: rawMultiCommand = %s\n", rawMultiCommand.c_str());

	// Parse the multi-command into a vector of commands
	vector<string> rawCommands = Helper::lex(rawMultiCommand, PIPE_DELIMITER);

	runCommands(rawCommands);
	return 0;
}