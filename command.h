#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
using namespace std;

class Command
{
public:
	static const char DELIMITER = ' ';

	Command(string rawCommand);														 // constructor
	Command(string rawCommand, int *inPipe, int *outPipe); // constructor
	void run();

private:
	string rawCommand;
	int *inPipe;
	int *outPipe;

	vector<string> tokenize(string rawCommand); //function that parses command line into strings
	void execute(vector<string> args);
	void childExecute(vector<string> args);
	void setInPipe(int *input);
	void setOutPipe(int *output);

	// Static helper function
	static char *const *stringVectorToCharArray(vector<string> toConvert);
};
