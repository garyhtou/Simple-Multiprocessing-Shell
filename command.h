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

	vector<string> parse(string rawCommand); //function that parses command line into strings
	void execute(vector<string> args);
	void childExecute(vector<string> args);
	void handleError(int code, string errorMessage);
	char*const *argChars(vector<string> toConvert);
};