#include "helper.h"

#include <string>

using namespace std;

string Helper::trimStr(string str)
{
	// Trim leading and trailing whitespace
	size_t first = str.find_first_not_of(" \t\n\r");
	size_t last = str.find_last_not_of(" \t\n\r");
	return str.substr(first, (last - first + 1));
}