#include "application.h"
#include "version.h"
#include <logger.h>
#include <SDL.h>
#ifdef _WIN32
#include <Windows.h>
#endif
#include "tools.h"

namespace spacetheory {
	application * application::s_app = nullptr;
}

using namespace spacetheory;

application::application()
{
#ifdef _WIN32
	// IF THE APPLICATION HAS A CONSOLE WINDOW, ENSURE IT ISN'T CLOSED
	// If the console window closes before the main window, it could prevent
	// the application from doing a complete clean-up.
	HWND hwnd = ::GetConsoleWindow();
	if (hwnd != NULL) {
		HMENU hMenu = ::GetSystemMenu(hwnd, FALSE);
		if (hMenu != NULL) DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
	}
#endif

	// ENSURE ONLY ONE APP OBJECT EXISTS:
	if (!this->s_app) this->s_app = this;
	else {
		const char * msg = "Only one application object can be created!";
		xeekworx::log << LOGSTAMP << xeekworx::logtype::FATAL << msg << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SPACETHEORY_FILEDESC, msg, NULL);
		throw std::exception();
	}

	// SETUP LOGGING:
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
	xeekworx::log << LOGSTAMP << xeekworx::DEBUG << L"Application Constructed" << std::endl;
}

application::~application()
{
	xeekworx::log << LOGSTAMP << xeekworx::DEBUG << L"Application Destructed" << std::endl;
}

int application::run(int argc, char *argv[])
{
	int exitcode = 0;

	// STARTUP STOPWATCH:
	auto start_clock = tools::clock::now();

	// COMMAND-LINE ARGUMENTS:
	// Convert the arguments as they come from main into something
	// better:
	std::vector<std::string> args;
	for (int i = 0; i < argc; ++i) args.push_back(argv[i]);

	// Log command-line:
	xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << L"Command-Line:" << std::endl;
	for (const auto &arg : args) {
		xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << L"+ " <<  arg.c_str() << L"" << std::endl;
	}

	// INITIALIZE APIS:
	xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << L"Setting up APIs ..." << std::endl;
	if (!setup_apis()) {
		const char * msg = "Failed to setup apis!";
		xeekworx::log << LOGSTAMP << xeekworx::logtype::FATAL << msg << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SPACETHEORY_FILEDESC, msg, NULL);

		exitcode = 1;
		return exitcode;
	}

	// APPLICATION'S START:
	// Where the game's initialization magic really happens. Game windows
	// are created here.
	if (!on_start(args, exitcode)) {
		const char * msg = "Application failed to start!";
		xeekworx::log << LOGSTAMP << xeekworx::logtype::FATAL << msg << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SPACETHEORY_FILEDESC, msg, NULL);
	}
	else {
		// STOPWATCH:
		auto end_clock = tools::clock::now();
		xeekworx::log << LOGSTAMP << L"Startup took " << tools::friendly_duration(start_clock, end_clock) << L" to complete" << std::endl;

		// GAME LOOP:
		game_loop();
	}

	// SHUTDOWN:
	auto start_shutdown_clock = tools::clock::now();
	xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << L"Shutting down APIs ..." << std::endl;
	close_apis();
	xeekworx::log << LOGSTAMP << L"Shutdown took " << tools::friendly_duration(start_shutdown_clock, tools::clock::now()) << L" to complete" << std::endl;

	// STOPWATCH:
	xeekworx::log << LOGSTAMP << L"Application uptime: " << tools::friendly_duration(start_clock, tools::clock::now()) << std::endl;

	// Returns 0 for success
	return exitcode;
}

bool application::setup_apis()
{
	// INITIALIZE SDL:
	SDL_version sdlvn = {};
	SDL_GetVersion(&sdlvn);
	xeekworx::log << LOGSTAMP << xeekworx::INFO << L"SDL Version: " << sdlvn.major << L"." << sdlvn.minor << L"." << sdlvn.patch << std::endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		xeekworx::log << LOGSTAMP << xeekworx::ERR << L"SDL Initialization Failed (" << SDL_GetError() << L")" << std::endl;
		return false;
	}
	else xeekworx::log << LOGSTAMP << xeekworx::DEBUG << L"SDL Initialized Successfully" << std::endl;

	return true;
}

void application::close_apis()
{
	// UNINITIALIZE SDL:
	SDL_Quit();
	xeekworx::log << LOGSTAMP << xeekworx::DEBUG << L"SDL Shutdown" << std::endl;
}

void application::game_loop()
{
	xeekworx::log << LOGSTAMP << xeekworx::NOTICE << L"Game Loop Started" << std::endl;

	// Nothing here yet.
}