#include "application.h"

using namespace spacetheory;

application::application()
{

}

application::~application()
{

}

int application::start(int argc, char *argv[])
{
	// Convert the arguments as they come from main into something
	// better:
	std::vector<std::string> args;
	for (int i = 0; i < argc; ++i) {
		args.push_back(argv[i]);
	}

	// Where the start magic really happens:
	int result = on_start(args);

	return result;
}