#include "application.h"
#include "version.h"
#include <logger.h>
#include <SDL.h>
#ifdef _WIN32
#include <Windows.h>
#endif
#include "tools.h"
#include "error.h"

namespace spacetheory {
	application * application::s_app = nullptr;
}

using namespace spacetheory;

application::application() : m_should_quit(false)
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
		throw spacetheory::error(msg);
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
	xeekworx::log << xeekworx::INFO << "Welcome to " << SPACETHEORY_PRODUCTNAME << " Engine, Version "
		<< vn.major << "." << vn.minor << "." << vn.revision << " " << SPACETHEORY_DEVSTAGE;
	xeekworx::log << xeekworx::INFO << " (Build " << vn.build << ")";
	xeekworx::log << std::endl;
	xeekworx::log << xeekworx::INFO << "Developed by " << SPACETHEORY_COMPANY << std::endl;
	xeekworx::log << xeekworx::INFO << "Platform Architecture: " << SPACETHEORY_ARCH_STR << std::endl;
	xeekworx::log << xeekworx::INFO << "Build Configuration: " << SPACETHEORY_CONFIG << std::endl;
	xeekworx::log << xeekworx::INFO << "Build Date: " << get_builddate().c_str() << std::endl;
	SDL_version sdlvn = {};
	SDL_GetVersion(&sdlvn);
	xeekworx::log << xeekworx::INFO << "SDL Version: " << sdlvn.major << "." << sdlvn.minor << "." << sdlvn.patch << std::endl;

	xeekworx::log << std::endl;

	// NORMAL LOGGING BEGINS:
	xeekworx::log.set_msgonly(false);
	xeekworx::log << LOGSTAMP << xeekworx::DEBUG2 << "Application Constructed" << std::endl;
}

application::~application()
{
	xeekworx::log << LOGSTAMP << xeekworx::DEBUG2 << "Application Destructed" << std::endl;
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
	xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << "Command-Line ..." << std::endl;
	unsigned i = 1;
	for (const auto &arg : args) {
		xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << "Arg " << i << " [" <<  arg.c_str() << "]" << std::endl;
		++i;
	}

	// INITIALIZE APIS:
	xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << "Setting up APIs ..." << std::endl;
	if (!setup_apis()) {
		const char * msg = "Failed to setup apis!";
		xeekworx::log << LOGSTAMP << xeekworx::logtype::FATAL << msg << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SPACETHEORY_FILEDESC, msg, NULL);

		exitcode = 1;
		return exitcode;
	}

	// APPLICATION'S START:
	// Where the game's startup magic really happens. Game windows
	// are created here.
	if (!on_start(args)) {
		const char * msg = "Application failed to start!";
		xeekworx::log << LOGSTAMP << xeekworx::logtype::FATAL << msg << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SPACETHEORY_FILEDESC, msg, NULL);
		exitcode = 1;
	}
	else {
		// STOPWATCH:
		auto end_clock = tools::clock::now();
		xeekworx::log << LOGSTAMP << "Startup took " << tools::friendly_duration(start_clock, end_clock) << " to complete" << std::endl;

		// GAME LOOP:
		game_loop();
	}

	// SHUTDOWN:
	auto start_shutdown_clock = tools::clock::now();
	xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << "Shutting down APIs ..." << std::endl;
	close_apis();
	xeekworx::log << LOGSTAMP << "Shutdown took " << tools::friendly_duration(start_shutdown_clock, tools::clock::now()) << " to complete" << std::endl;

	// STOPWATCH:
	xeekworx::log << LOGSTAMP << "Application uptime: " << tools::friendly_duration(start_clock, tools::clock::now()) << std::endl;

	// Returns 0 for success
	return exitcode;
}

bool application::setup_apis()
{
	// INITIALIZE SDL:
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		xeekworx::log << LOGSTAMP << xeekworx::ERR << "SDL Initialization Failed (" << SDL_GetError() << ")" << std::endl;
		return false;
	}
	else xeekworx::log << LOGSTAMP << xeekworx::DEBUG << "SDL Initialized Successfully" << std::endl;

	return true;
}

void application::close_apis()
{
	// UNINITIALIZE SDL:
	SDL_Quit();
	xeekworx::log << LOGSTAMP << xeekworx::DEBUG << "SDL Shutdown" << std::endl;
}

void application::shutdown()
{
	m_should_quit = true;
	xeekworx::log << LOGSTAMP << xeekworx::DEBUG2 << "Shutdown called!" << std::endl;
}

void application::game_loop()
{
	xeekworx::log << LOGSTAMP << xeekworx::NOTICE << "Game Loop Started" << std::endl;

	while (!this->m_should_quit) {
		// Empty the event queue entirely:
		if (!event_loop()) break;

		// Rendering magic:
		on_frame();
	}

	xeekworx::log << LOGSTAMP << xeekworx::NOTICE << "Game Loop Ended" << std::endl;
}

bool application::event_loop()
{
	static SDL_Event e = {};
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			return false;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_ESCAPE) return false;
			break;
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
			break;
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEWHEEL:
			break;
		case SDL_WINDOWEVENT:
			break;
		}
	}

	// Return true to keep the game loop running.
	return true;
}

void application::on_frame()
{

}