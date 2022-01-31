#include "command.h"
#include "helper.h"

#include <iostream>

using namespace std;

const char PIPE_DELIMITER = '|';



/*
 * IMPORTANT!
 * This parsing function will not ignore pipes within strings.
 * TODO: Do we need to support that?
 */
void  parse(string rawMultiCommand)
{
	printf("\tDEBUG: parse\n");

	rawMultiCommand = Helper::trimStr(rawMultiCommand);
	vector<string> commands;

	if (rawMultiCommand.length() == 0)
	{
		printf("\tDEBUG: parse: rawMultiCommand is empty\n");
		//return commands;

	}

	else{

		// TODO: parse needs to account for pipes within strings
		size_t index = 0;
		string currCommand;

		//check for pipes and run pipe path if found
		if (rawMultiCommand.find(PIPE_DELIMITER) != string::npos) {
					
			/* Split using the delimiter (BLOCKED FOR NOW: PASSING RAW STRING
			   TO COMMAND CONSTRUCTOR FOR PIPED ARGUMENTS)
			*/
			/* while ((index = rawMultiCommand.find(PIPE_DELIMITER)) != string::npos)
			{
				currCommand = rawMultiCommand.substr(0, index);
				commands.push_back(Helper::trimStr(currCommand));
				printf("\tDEBUG: currPipedToken = %s\n", currCommand.c_str());
				rawMultiCommand.erase(0, index + 1);
			}
				// push the last command
				commands.push_back(rawMultiCommand);
				printf("\tDEBUG: currPipedToken = %s\n", rawMultiCommand.c_str()); */
				
				//call piped command runner
				runPipedCommands(rawMultiCommand);
				
		}else{ 
			
			while ((index = rawMultiCommand.find(" ")) != string::npos)
			{
				currCommand = rawMultiCommand.substr(0, index);
				commands.push_back(Helper::trimStr(currCommand));
				printf("\tDEBUG: currCommand = %s\n", currCommand.c_str());
				rawMultiCommand.erase(0, index + 1);
			}
				// push the last command
				commands.push_back(rawMultiCommand);
				printf("\tDEBUG: currCommand = %s\n", rawMultiCommand.c_str());

		}
	}


}

void runCommands(vector<string> rawCommands)
{
	printf("\tDEBUG: runCommands\n");
	vector<Command> commands;

	for (int i = 0; i < rawCommands.size(); i++)
	{
		// Create a new command
		Command command = Command(rawCommands[i]);
		commands.push_back(command);

		// Run the command
		command.run();
	}
}

void runPipedCommands(string rawCommands){
	printf("\tDEBUG: runPipedCommands\n");
	vector<Command> commands;

	// Create a new command
	Command command = Command(rawCommands);
	//ru excecute pipedcommands
	command.runPiped();
}

int main(int argc, char *argv[])
{
	printf("\tDEBUG: main\n");
	// Get multi-command from standard input
	string rawMultiCommand;
	getline(cin, rawMultiCommand);

	printf("\tDEBUG: rawMultiCommand = %s\n", rawMultiCommand.c_str());

	// Parse the multi-command into a vector of commands
	//vector<string> rawCommands = parse(rawMultiCommand);

	//runCommands(rawCommands);

	// Command command("cat lksdf");
	// Command command("echo hi");
	// command.run();
	return 0;
}