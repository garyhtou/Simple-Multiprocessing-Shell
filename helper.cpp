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
	char currQuote = '\0';
	bool last = false;

	while (!last)
	{
		try
		{
			// Find next delimiter character
			index = rawCommand.find(delimiter);

			if (index != string::npos)
			{
				// Set the current word
				currWord = rawCommand.substr(0, index);
				// Remove the current word from the raw command
				rawCommand.erase(0, index + 1);
			}
			// If a delimiter is not found
			else
			{
				// The rest of the string is the last word
				currWord = rawCommand;
				// set flag to signify that this is the last word
				last = true;
			}

			// If we are currently in a quoted string
			if (currQuote != '\0')
			{
				// Check if the quoted string is ending.
				// The following conditions must be true:
				//   1. The last (or only) character in the string is the same quote
				//      character that was used to start the quoted string.
				//   2. The character (if existint) before the quote character is not an
				//      escape character.
				if ((currWord.back() == currQuote) &&
						(currWord.size() == 1 || (currWord.size() >= 2 && currWord[-2] != ESCAPE_CHAR)))
				{
					// Reset the current quote character to signify that we are no longer
					// in a quoted string
					currQuote = '\0';

					// If the ending quote is attached to other text
					if (currWord.size() > 1)
					{
						// Remove the ending quote character from the string and add to the
						// current token
						currWord.pop_back();
						currToken += currWord;
					}

					// Add the completed token to the vector
					tokens.push_back(currToken); // Don't trim this token
				}
				else
				{
					// Are we still inside the quotes. Add the current word to the
					// pre-existing current token
					currToken += currWord + delimiter;

					// Normally, at this point we would not add the current token to the
					// vector since it is incomplete (the quoted string has not ended)
					//
					// However, if this is the last word in the command, and we are at
					// this state, then this means that quote string was never terminated.
					// Therefore, to prevent token loss, we add the current token to the
					// vector (effectively closing the quoted string for them)
					if (last)
					{
						tokens.push_back(currToken); // Don't trim this token
					}
				}
			}
			// This else case handles words that are not inside quoted strings
			else
			{
				// Skip current word if it's just whitespace
				if (trimStr(currWord).size() == 0)
				{
					continue;
				}

				// If the first character of the word is a quote character, being a new
				// token and update the current quote character to signify that we are
				// now in a quoted string
				//
				// However, if this is the last word, don't allow creation of a new
				// string. Just add the whole word as a token (in the else statement)
				if (!last &&
						(currWord.front() == DOUBLE_QUOTE || currWord.front() == SINGLE_QUOTE))
				{
					// We are starting a new quote
					currQuote = currWord.front();
					// Remove the quote character from the begining of the current word
					currWord.erase(0, 1);
					currToken = currWord + delimiter; // Preseve the delimiter
				}
				else
				{
					// Otherwise just add the word as a token to the vector
					currToken = currWord;
					tokens.push_back(trimStr(currToken));
				}
			}
		}
		catch (...)
		{
			// do nothing
			// word is skipped
		}
	}

	// Return all the lexed tokens
	return tokens;
}