#include "helper.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

string Helper::trimStr(string str)
{
	// Trim leading and trailing whitespace
	size_t first = str.find_first_not_of(" \t\n\r");
	size_t last = str.find_last_not_of(" \t\n\r");
	return str.substr(first, (last - first + 1));
}

int Helper::debugPrint(const char *message)
{
	return printf("\tDEBUG: %s\n", message);
}

const char DOUBLE_QUOTE = '"';
const char SINGLE_QUOTE = '\'';
const char ESCAPE_CHAR = '\\';

vector<string> Helper::lex(string rawCommand, char delimiter)
{
	Helper::debugPrint("Helper::tokenize");
	printf("\tDEBUG: rawCommand = %s\n", rawCommand.c_str());
	rawCommand = Helper::trimStr(rawCommand);

	vector<string> tokens;

	size_t index = 0;
	string currToken;
	string currWord;
	bool inQuotes = false;
	char currQuote;

	// Split using the delimiter
	while ((index = rawCommand.find(delimiter)) != string::npos)
	{
		currWord = rawCommand.substr(0, index);

		// If we are currently in quotes
		if (inQuotes)
		{
			if ((currWord.back() == currQuote) && (currWord.size() > 1 && currWord[-2] != ESCAPE_CHAR))
			{
				inQuotes = false;
				currQuote = '\0';
				// Add word to current token without the last char
				currToken += currWord.substr(0, currWord.size() - 1);
				tokens.push_back(Helper::trimStr(currToken));
				printf("\tDEBUG: currToken = %s\n", tokens.back().c_str());
			}
			else
			{
				currToken += currWord + delimiter;
			}
		}
		else
		{
			if (currWord.front() == DOUBLE_QUOTE || currWord.front() == SINGLE_QUOTE)
			{
				inQuotes = true;
				currQuote = currWord.front();
				currWord.erase(0, 1);
				currToken = currWord + delimiter;
			}
			else
			{
				currToken = currWord;
				tokens.push_back(Helper::trimStr(currToken));
				printf("\tDEBUG: currToken = %s\n", tokens.back().c_str());
			}
		}

		rawCommand.erase(0, index + 1);
	}

	// Ending case
	currWord = rawCommand;
	if (inQuotes)
	{
		if ((currWord.back() == currQuote) && (currWord.size() > 1 && currWord[-2] != ESCAPE_CHAR))
		{
			inQuotes = false;
			currQuote = '\0';
			// Add word to current token without the last char
			currToken += currWord.substr(0, currWord.size() - 1);
			tokens.push_back(Helper::trimStr(currToken));
			printf("\tDEBUG: currToken = %s\n", tokens.back().c_str());
		}
		else
		{
			currToken += currWord + delimiter;
			// String never ended, but we'll end it anyways
			tokens.push_back(Helper::trimStr(currToken));
			printf("\tDEBUG: currToken = %s\n", tokens.back().c_str());
		}
	}
	else
	{
		currToken = currWord;
		tokens.push_back(Helper::trimStr(currToken));
		printf("\tDEBUG: currToken = %s\n", tokens.back().c_str());
	}

	return tokens;
}

// inQuote is a boolean represent if we're currently in a quote

// if inQuote
// 	if word's last char is quote (and 2nd to last char, if exists, is not escape char)
// 		(inQuote = false)
// 		add word (without last char (quote)) to current token
// 	else
// 		add word to current token
// else
// 	if word's first char is quote
// 		(inQuote = true)
// 		add word (without first char (quote)) as new token
// 	else
// 		add word as new token
//
// NOTE: whenever we add to the current token, we need to make sure we keep the delimiter (space, pipe, etc.)