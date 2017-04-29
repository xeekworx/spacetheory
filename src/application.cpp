#include "application.h"
#include "version.h"
#include <logger.h>
#include <SDL.h>

using namespace spacetheory;

application::application()
{
	xeekworx::logger::config log_config = xeekworx::logger::default_config();
	log_config.file = SPACETHEORY_LOGFILE;
	log_config.output_to_file = true;
	log_config.output_to_vs = true;
	xeekworx::log.enable(log_config);

	// LOG GAME ENGINE INFO:
	// Do so without timestamps, etc.
	xeekworx::log.set_msgonly(true);
	version vn = get_version();
	xeekworx::log << xeekworx::INFO << L"Welcome to " << SPACETHEORY_PRODUCTNAME << L" Version "
		<< vn.major << L"." << vn.minor << L"." << vn.revision << L" " << SPACETHEORY_DEVSTAGE;
	xeekworx::log << xeekworx::INFO << L" (Build " << vn.build << L")";
	xeekworx::log << std::endl;
	xeekworx::log << xeekworx::INFO << L"Developed by " << SPACETHEORY_COMPANY << std::endl;
	xeekworx::log << xeekworx::INFO << L"Platform Architecture: " << SPACETHEORY_ARCH_STR << std::endl;
	xeekworx::log << xeekworx::INFO << L"Build Configuration: " << SPACETHEORY_CONFIG << std::endl;
	xeekworx::log << std::endl;

	// NORMAL LOGGING BEGINS:
	xeekworx::log.set_msgonly(false);
	xeekworx::log << LOGSTAMP << xeekworx::DEBUG << L"Application constructed" << std::endl;
}

application::~application()
{
	xeekworx::log << LOGSTAMP << xeekworx::DEBUG << L"Application destructed" << std::endl;
}

int application::start(int argc, char *argv[])
{
	int exitcode = 0;

	// Convert the arguments as they come from main into something
	// better:
	std::vector<std::string> args;
	for (int i = 0; i < argc; ++i) args.push_back(argv[i]);

	// Log command-line:
	xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << L"Command-Line:" << std::endl;
	for (const auto &arg : args) {
		xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << L"+ " <<  arg.c_str() << L"" << std::endl;
	}

	// Where the game's initialization magic really happens. Game windows
	// are created here.
	if (!on_start(args, exitcode)) {
		xeekworx::log << LOGSTAMP << xeekworx::logtype::FATAL << L"Application failed to start!" << std::endl;
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