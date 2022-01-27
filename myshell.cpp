#include "command.h"

int main(int argc, char *argv[])
{
	Command command("echo hi");
	command.run();
	return 0;
}