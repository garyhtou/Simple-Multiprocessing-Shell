#include "command.h"
#include "helper.h"

#include <iostream>
#include <unistd.h>

using namespace std;

const char PIPE_DELIMITER = '|';

void waitForChildren()
{
	while (true)
	{
		// Wait for the child process to exit
		int status;
		pid_t pid = wait(&status);

		if (int(pid) <= 0)
		{
			// No children left. We have waited for all children to exit
			return;
		}

		// Get and print exit status of child process
		if (WIFEXITED(status))
		{
			int code = WEXITSTATUS(status);
			cout << "process " << pid << " exits with " << code << endl;
		}
		// Handle edge case where child process was terminated via signal
		else if (WIFSIGNALED(status))
		{
			int signal = WTERMSIG(status);
			// The process was terminated by a signal.
			cout << "process " << pid << " was terminated with " << signal << endl;
		}
		else
		{
			cout << "process " << pid << " exits with "
					 << "unknown code" << endl;
		}
	}
}

void runCommands(vector<string> rawCommands)
{
	// Handle single command (no pipes)
	if (int(rawCommands.size()) == 1)
	{
		// Run the single command
		Command command = Command(rawCommands.front());
		command.run();

		// Wait for the single child process to exit
		waitForChildren();
		return;
	}

	// Everything from here on is for multi-commands (involving pipes)

	// Set up array to keep track of pipes
	int inPipe[2];
	int outPipe[2];

	// Set up pipe for first command
	if (pipe(outPipe) == -1)
	{
		cout << "Error: pipe failed" << endl;
	};

	// Run the first command with only an output pipe
	Command leadingCommand = Command(rawCommands[0], NULL, outPipe);
	leadingCommand.run();

	// Reroute the pipes for the next command
	inPipe[0] = outPipe[0];
	inPipe[1] = outPipe[1];

	// Start with the 2nd command and end with the 2nd to last command since the
	// first and last commands use default STDIN/STDOUT
	for (size_t i = 1; i < rawCommands.size() - 1; i++)
	{
		// Set up the pipes
		if (pipe(outPipe) == -1)
		{
			cout << "Error: pipe failed" << endl;
		};
		// Run the command
		Command command = Command(rawCommands[i], inPipe, outPipe);
		command.run();

		// Reroute the pipes for the next command
		inPipe[0] = outPipe[0];
		inPipe[1] = outPipe[1];
	}

	// Run the last command
	Command trailingCommand = Command(rawCommands[rawCommands.size() - 1], inPipe, NULL);
	trailingCommand.run();

	// Wait for children AFTER forking and executing all commands
	waitForChildren();
}

int main(int argc, char *argv[])
{
	// Display prompt and wait for input
	printf("myshell$");

	// Get multi-command from standard input
	string rawMultiCommand;
	getline(cin, rawMultiCommand);

	// Parse the multi-command into a vector of raw commands (strings)
	vector<string> rawCommands = Helper::lex(rawMultiCommand, PIPE_DELIMITER);

	try
	{
		runCommands(rawCommands);
	}
	catch (...)
	{
		cout << "An error has occured";
	}
	return 0;
}