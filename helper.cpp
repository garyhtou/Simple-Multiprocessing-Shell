#include "helper.h"
#include <string>
#include <vector>

using namespace std;

string Helper::trimStr(string str)
{
	try
	{
		// Trim leading and trailing whitespace
		size_t first = str.find_first_not_of(" \t\n\r");
		size_t last = str.find_last_not_of(" \t\n\r");

		// Handle case where no non-whitespace characters are found
		if (first == -1)
		{
			first = 0;
		}
		if (last == -1)
		{
			last = str.length();
		}
		return str.substr(first, (last - first + 1));
	}
	catch (...)
	{
		// Return input string is an error occurs
		return str;
	}
}

vector<string> Helper::lex(string rawCommand, char delimiter)
{
	// Remove leading and trailing whitespace from the raw command
	rawCommand = trimStr(rawCommand);

	// Set up a vector to hold the tokens
	vector<string> tokens;

	// Variables for tracking the current token/word
	size_t index = 0;
	string currToken;
	string currWord;
	bool inQuotes = false;
	char currQuote;

	// Split using the delimiter
	while ((index = rawCommand.find(delimiter)) != string::npos)
	{
		try
		{ //create a word and remove the space
			currWord = rawCommand.substr(0, index);
			rawCommand.erase(0, index + 1);

			// If we are currently in quotes
			if (inQuotes)
			{
				bool endQuote = false;
				try
				{ //store the ending quote type if not preceded by the escape character
					endQuote = (currWord.back() == currQuote) &&
										 (currWord.size() == 1 || currWord[-2] != ESCAPE_CHAR);
				}
				catch (...)
				{
					// do nothing
				}
				if (endQuote)
				{
					inQuotes = false;
					currQuote = '\0';
					// Add word to current token without the quotation to the token
					currToken += currWord.substr(0, currWord.size() - 1);
					tokens.push_back(trimStr(currToken));
				}
				else
				{
					// Are we still inside the quotes
					currToken += currWord + delimiter;
				}
			}
			else
			{
				// Skip current word if it's just whitespace
				if (trimStr(currWord).size() == 0)
				{
					continue;
				}

				// if the first character of the word is double quotations,
				// erase the quotations and update the token
				if (currWord.front() == DOUBLE_QUOTE || currWord.front() == SINGLE_QUOTE)
				{
					// We are starting a new quote
					inQuotes = true;
					currQuote = currWord.front();
					currWord.erase(0, 1);
					currToken = currWord + delimiter;
				}
				else
				{
					// otherwise just add the word to the vector
					currToken = currWord;
					tokens.push_back(trimStr(currToken));
				}
			}
		}
		catch (...)
		{
			// do nothing
		}
	}

	// Ending case
	currWord = rawCommand;
	if (inQuotes)
	{
		bool endQuote = false;
		try
		{
			endQuote = (currWord.back() == currQuote) &&
								 (currWord.size() == 1 || currWord[-2] != ESCAPE_CHAR);
		}
		catch (...)
		{
			// do nothing
		}
		if (endQuote)
		{
			inQuotes = false;
			currQuote = '\0';
			// Add word to current token without the last char
			currToken += currWord.substr(0, currWord.size() - 1);
			tokens.push_back(trimStr(currToken));
		}
		else
		{
			currToken += currWord + delimiter;
			// String never ended, but we'll end it anyways
			tokens.push_back(trimStr(currToken));
		}
	}
	// Skip current word if it's just whitespace
	else if (trimStr(currWord).size() > 0)
	{
		currToken = currWord;
		tokens.push_back(trimStr(currToken));
	}

	// Return all the lexed tokens
	return tokens;
}