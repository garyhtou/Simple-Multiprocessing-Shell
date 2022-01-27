//

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
using namespace std;

class Command
{
public:
	Command(string rawCommand); // constructor
	~Command();									// destructor
	void run();
	static const char DELIMITER = ' ';

private:
	string rawCommand;

	vector<string> tokenize(string rawCommand); //function that parses command line into strings
	void execute(vector<string> args);
	void childExecute(vector<string> args);

	// Static helper function
	static char *const *stringVectorToCharArray(vector<string> toConvert);
};