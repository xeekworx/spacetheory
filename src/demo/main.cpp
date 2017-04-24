#include "demo_application.h"

int main(int argc, char *argv[])
{
	demo_application * app = new demo_application();

	int exitcode = app->start(argc, argv);

	delete app;
	app = nullptr;

	return exitcode;
}