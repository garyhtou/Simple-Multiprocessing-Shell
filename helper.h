#include <string>
#include <vector>

using namespace std;

namespace Helper
{
	const char DOUBLE_QUOTE = '"';
	const char SINGLE_QUOTE = '\'';
	const char ESCAPE_CHAR = '\\';

	string trimStr(string str);
	vector<string> lex(string rawMultiCommand, char delimiter);
}