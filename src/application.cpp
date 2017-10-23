#include "application.h"
#include "version.h"
#include <logger.h>
#include <SDL.h>
//#include <SDL_opengl.h> // Do not include this when using glad
#ifdef _WIN32
#include <Windows.h>
#endif
#include <glad\glad.h>
#include "tools.h"
#include "error.h"
#include <unordered_map>
#include <iomanip>

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
	// I have to cast version values as unsigned int or they'll be displayed as characters
	xeekworx::log << xeekworx::INFO << "SDL Version: " 
		<< static_cast<unsigned>(sdlvn.major)
		<< "." 
		<< static_cast<unsigned>(sdlvn.minor)
		<< "." 
		<< static_cast<unsigned>(sdlvn.patch)
		<< std::endl;

	xeekworx::log << std::endl;

	// NORMAL LOGGING BEGINS:
	xeekworx::log.set_msgonly(false);
	xeekworx::log << LOGSTAMP << xeekworx::DEBUG2 << "Application constructed" << std::endl;
}

application::~application()
{
	xeekworx::log << LOGSTAMP << xeekworx::DEBUG2 << "Application destructed" << std::endl;
}

int application::run(int argc, char *argv[])
{
	int exitcode = 0;

	// STARTUP STOPWATCH:
	auto start_clock = tools::clock::now();

	// COMMAND-LINE ARGUMENTS:
	// Convert the arguments as they come from main into something better:
	std::vector<std::string> args;
	for (int i = 0; i < argc; ++i) args.push_back(argv[i]);

	// Log command-line:
	xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << "Command-Line ..." << std::endl;
	unsigned i = 1;
	for (const auto &arg : args) {
		xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << "> arg " << i << " [" <<  arg.c_str() << "]" << std::endl;
		++i;
	}

	// CONFIGURE APIS (STAGE 1):
	xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << "Configuring APIs (Stage 1) ..." << std::endl;
	if (!setup_apis1()) {
		const char * msg = "Failed in API Configuration Stage 1!";
		xeekworx::log << LOGSTAMP << xeekworx::logtype::FATAL << msg << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SPACETHEORY_FILEDESC, msg, NULL);

		exitcode = 1;
		return exitcode;
	}

	// APPLICATION'S START:
	// Where the game's startup magic really happens. Game window setup and 
	// graphics setup are done here.
	display_setup disp_setup;
	graphics_setup gfx_setup;
	if (!on_start(args, disp_setup, gfx_setup)) {
		const char * msg = "Application failed to start!";
		xeekworx::log << LOGSTAMP << xeekworx::logtype::FATAL << msg << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SPACETHEORY_FILEDESC, msg, NULL);
		exitcode = 1;
	}
	else {
		// CONFIGURE APIS (STAGE 2):
		xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << "Configuring APIs (Stage 2) ..." << std::endl;
		if (!setup_apis2(gfx_setup)) {
			xeekworx::log << LOGSTAMP << xeekworx::logtype::WARNING << "Failed in API Configuration Stage 2!" << std::endl;
			exitcode = 1;
		}

		// CREATE DISPLAY (GAME WINDOW):
		xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << "Creating display (game window) ..." << std::endl;
		if (!create_display(disp_setup)) {
			// create_display should take care of any error messages.
			exitcode = 1;
		}
		else {
			// CONFIGURE APIS (STAGE 3):
			xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << "Configuring APIs (Stage 3) ..." << std::endl;
			if (!setup_apis3(gfx_setup)) {
				xeekworx::log << LOGSTAMP << xeekworx::logtype::WARNING << "Failed in API Configuration Stage 3!" << std::endl;
				exitcode = 1;
			}

			// STOPWATCH:
			auto end_clock = tools::clock::now();
			xeekworx::log << LOGSTAMP << "Startup took " << tools::friendly_duration(start_clock, end_clock) << " to complete" << std::endl;

			// GAME LOOP:
			game_loop();
		}
	}

	// SHUTDOWN:
	auto start_shutdown_clock = tools::clock::now();
	xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << "Shutting down APIs ..." << std::endl;
	shutdown_apis();
	if (m_display) { // Destroy the game window
		delete m_display;
		m_display = nullptr;
	}
	xeekworx::log << LOGSTAMP << "Shutdown took " << tools::friendly_duration(start_shutdown_clock, tools::clock::now()) << " to complete" << std::endl;

	// STOPWATCH:
	xeekworx::log << LOGSTAMP << "Application uptime: " << tools::friendly_duration(start_clock, tools::clock::now()) << std::endl;

	// Returns 0 for success
	return exitcode;
}

bool application::create_display(const display_setup& setup)
{
	try {
		this->m_display = new spacetheory::display(setup);
	}
	catch (...) {
		const std::string msg = "Failed to create display (game window)!";
		xeekworx::log << LOGSTAMP << xeekworx::logtype::FATAL << msg << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SPACETHEORY_FILEDESC, msg.c_str(), NULL);
		return false;
	}

	return true;
}

bool application::setup_apis1()
{
	// INITIALIZE SDL:
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		xeekworx::log << LOGSTAMP << xeekworx::ERR << "SDL Initialization Failed (" << SDL_GetError() << ")" << std::endl;
		return false;
	}
	else xeekworx::log << LOGSTAMP << xeekworx::DEBUG << "SDL Initialized Successfully" << std::endl;



	return true;
}

bool application::setup_apis2(const graphics_setup& gfx_setup)
{
	bool result = true; // true for success

	// CONFIGURE OPENGL ATTRIBUTES:
	std::unordered_map<SDL_GLattr, int> attributes;
	attributes[SDL_GL_CONTEXT_FLAGS] = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
	attributes[SDL_GL_CONTEXT_PROFILE_MASK] = SDL_GL_CONTEXT_PROFILE_CORE;
	attributes[SDL_GL_CONTEXT_MAJOR_VERSION] = 4;
	attributes[SDL_GL_CONTEXT_MINOR_VERSION] = 1;
	attributes[SDL_GL_RED_SIZE] = 8;
	attributes[SDL_GL_GREEN_SIZE] = 8;
	attributes[SDL_GL_BLUE_SIZE] = 8;
	attributes[SDL_GL_ALPHA_SIZE] = 8;
	attributes[SDL_GL_DEPTH_SIZE] = 24;
	attributes[SDL_GL_STENCIL_SIZE] = 8;
	attributes[SDL_GL_DOUBLEBUFFER] = 1;
	attributes[SDL_GL_MULTISAMPLEBUFFERS] = gfx_setup.msaa ? 1 : 0;
	attributes[SDL_GL_MULTISAMPLESAMPLES] = gfx_setup.msaa_samples;
	attributes[SDL_GL_SHARE_WITH_CURRENT_CONTEXT] = 1;

	xeekworx::log << LOGSTAMP << xeekworx::DEBUG << "Configuring OpenGL Attributes ... " << std::endl;
	for (auto i = attributes.begin(); i != attributes.end(); ++i) {
		result |= (0 != SDL_GL_SetAttribute((*i).first, (*i).second));
		if (!result) xeekworx::log << LOGSTAMP << xeekworx::ERR << "> FAIL : ";
		else xeekworx::log << LOGSTAMP << xeekworx::DEBUG2 << "> OK : ";
		xeekworx::log << std::setw(34) << std::left << sdltools::SDL_GLattrToString((*i).first) << " = ";
		switch ((*i).first) {
		case SDL_GL_CONTEXT_FLAGS: xeekworx::log << sdltools::SDL_GLcontextFlagToString((*i).second); break;
		case SDL_GL_CONTEXT_PROFILE_MASK: xeekworx::log << sdltools::SDL_GLprofileToString((*i).second); break;
		case SDL_GL_CONTEXT_RELEASE_BEHAVIOR: xeekworx::log << sdltools::SDL_GLcontextReleaseFlagToString((*i).second); break;
		default: xeekworx::log << (*i).second; break;
		}
		xeekworx::log << std::endl;
	}

	return result;
}

bool application::setup_apis3(const graphics_setup& gfx_setup)
{


	return true;
}

void application::shutdown_apis()
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