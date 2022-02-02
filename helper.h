#include <string>
#include <algorithm>

using namespace std;

namespace Helper
{
	const char DOUBLE_QUOTE = '"';
	const char SINGLE_QUOTE = '\'';
	const char ESCAPE_CHAR = '\\';

	string trimStr(string str);
	int debugPrint(const char *message);
	vector<string> lex(string rawMultiCommand, char delimiter);
}