#include "application.h"
#include "version.h"
#include <logger.h>
#include <SDL.h>

using namespace spacetheory;

application::application()
{
	xeekworx::logger::config log_config = xeekworx::logger::default_config();
	log_config.file = SPACETHEORY_LOGFILE;
	log_config.enable = true;
	xeekworx::log.enable(log_config);

	xeekworx::log << LOGSTAMP << xeekworx::logtype::INFO << L"Application constructed" << std::endl;
}

application::~application()
{
	xeekworx::log << LOGSTAMP << xeekworx::logtype::INFO << L"Application destructed" << std::endl;
}

int application::start(int argc, char *argv[])
{
	int exitcode = 0;

	// Convert the arguments as they come from main into something
	// better:
	std::vector<std::string> args;
	for (int i = 0; i < argc; ++i) {
		args.push_back(argv[i]);
	}

	// Where the game's initialization magic really happens. Game windows
	// are created here.
	if (!on_start(args, exitcode)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Space Theory Engine", "The application failed to start!", NULL);
	}
	else {
		// Game loop:
		run();
	}

	// Returns 0 for success
	return exitcode;
}

void application::run()
{
	// Nothing here yet.
}