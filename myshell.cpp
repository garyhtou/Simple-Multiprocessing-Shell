#include "command.h"
#include "helper.h"

#include <unistd.h>
#include <iostream>

using namespace std;

const char PIPE_DELIMITER = '|';

/*
 * IMPORTANT!
 * This parsing function will not ignore pipes within strings.
 * TODO: Do we need to support that?
 */
vector<string> parse(string rawMultiCommand)
{
	printf("\tDEBUG: parse\n");

	rawMultiCommand = Helper::trimStr(rawMultiCommand);
	rawMultiCommand = Helper::removeQuotes(rawMultiCommand);
	vector<string> commands;

	if (rawMultiCommand.length() == 0)
	{
		printf("\tDEBUG: parse: rawMultiCommand is empty\n");
		return commands;
	}
	else
	{
		// TODO: parse needs to account for pipes within strings
		size_t index = 0;
		string currCommand;

		while ((index = rawMultiCommand.find(PIPE_DELIMITER)) != string::npos)
		{
			currCommand = rawMultiCommand.substr(0, index);
			commands.push_back(Helper::trimStr(currCommand));
			printf("\tDEBUG: currPipedToken = %s\n", commands.back().c_str());
			rawMultiCommand.erase(0, index + 1);
		}
		// push the last command
		commands.push_back(Helper::trimStr(rawMultiCommand));

		
		printf("\tDEBUG: currPipedToken = %s\n", commands.back().c_str());
		return commands;
	}
}




void runCommands(vector<string> rawCommands)
{
	printf("\tDEBUG: runCommands\n");

	if (rawCommands.size() == 1)
	{
		printf("\tDEBUG: runCommands: Running SINGLE command\n");
		Command command = Command(rawCommands[0], NULL, NULL);
		command.run();
		return;
	}

	printf("\tDEBUG: runCommands: Running MULTI command\n");
	vector<Command> commands;

	int inPipe[2];
	int outPipe[2];

	// Set up and run the first command
	printf("\tDEBUG: runCommands: first command\n");
	pipe(outPipe);

	Command leadingCommand = Command(rawCommands[0], NULL, outPipe);
	commands.push_back(leadingCommand);
	leadingCommand.run();

	inPipe[0] = outPipe[0];
	inPipe[1] = outPipe[1];

	// Start with the 2nd command and end with the 2nd to last command since the
	// first and last commands use default STDIN/STDOUT
	for (int i = 1; i < rawCommands.size() - 1; i++)
	{
		printf("\tDEBUG: runCommands: command #%d\n", i + 1);
		pipe(outPipe);
		// Create a new command
		Command command = Command(rawCommands[i], inPipe, outPipe);
		commands.push_back(command);
		command.run();

		close(inPipe[0]);
		close(inPipe[1]);

		inPipe[0] = outPipe[0];
		inPipe[1] = outPipe[1];
	}

	// Set up and run the last command
	printf("\tDEBUG: runCommands: last command\n");
	Command trailingCommand = Command(rawCommands[rawCommands.size() - 1], inPipe, NULL);
	commands.push_back(trailingCommand);
	trailingCommand.run();

	close(inPipe[0]);
	close(inPipe[1]);
}

int main(int argc, char *argv[])
{
	printf("\tDEBUG: main\n");
	// Get multi-command from standard input
	string rawMultiCommand;
	getline(cin, rawMultiCommand);

	printf("\tDEBUG: rawMultiCommand = %s\n", rawMultiCommand.c_str());



	// Parse the multi-command into a vector of commands
	vector<string> rawCommands = parse(rawMultiCommand);

	runCommands(rawCommands);

	// Command command("cat lksdf");
	// Command command("echo hi");
	// command.run();
	return 0;
}