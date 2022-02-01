#include <string>
#include <algorithm>


using namespace std;

namespace Helper
{
	string trimStr(string str);
	int debugPrint(const char *message);
	vector<string> lex(string rawMultiCommand, char delimiter);
}