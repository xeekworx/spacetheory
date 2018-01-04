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
#include "graphics2d.h"

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

	// LOG OPENGL ERRORS:
	check_glerrors();

	// SHUTDOWN:
	auto start_shutdown_clock = tools::clock::now();
	xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << "Destroying display (game window) ..." << std::endl;
	if (m_display) { // Destroy the game window
		delete m_display;
		m_display = nullptr;
	}
	xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << "Shutting down APIs ..." << std::endl;
	shutdown_apis();

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
    // ----------------------------------------------------------------------------
	// STAGE 1, before any apis have initialized
    // ----------------------------------------------------------------------------

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
	// ----------------------------------------------------------------------------
	// STAGE 2, before the window (display) is created
	// ----------------------------------------------------------------------------

	bool result = true; // true for success

	// CONFIGURE OPENGL ATTRIBUTES:
	std::unordered_map<SDL_GLattr, int> attributes;
#ifdef _DEBUG
	attributes[SDL_GL_CONTEXT_FLAGS] = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG | SDL_GL_CONTEXT_DEBUG_FLAG;
#else
	attributes[SDL_GL_CONTEXT_FLAGS] = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
#endif
	attributes[SDL_GL_CONTEXT_PROFILE_MASK] = SDL_GL_CONTEXT_PROFILE_CORE;
	attributes[SDL_GL_CONTEXT_MAJOR_VERSION] = gfx_setup.glversion_major;
	attributes[SDL_GL_CONTEXT_MINOR_VERSION] = gfx_setup.glversion_minor;
	attributes[SDL_GL_RED_SIZE] = 8;
	attributes[SDL_GL_GREEN_SIZE] = 8;
	attributes[SDL_GL_BLUE_SIZE] = 8;
	attributes[SDL_GL_ALPHA_SIZE] = 8;
	attributes[SDL_GL_DEPTH_SIZE] = 24;
	attributes[SDL_GL_STENCIL_SIZE] = 8;
	attributes[SDL_GL_DOUBLEBUFFER] = 1;
	attributes[SDL_GL_MULTISAMPLEBUFFERS] = gfx_setup.msaa ? 1 : 0;
	attributes[SDL_GL_MULTISAMPLESAMPLES] = gfx_setup.msaa_samples;
	//attributes[SDL_GL_SHARE_WITH_CURRENT_CONTEXT] = 1;

	xeekworx::log << LOGSTAMP << xeekworx::DEBUG << "Configuring OpenGL Attributes ... " << std::endl;
	for (auto i = attributes.begin(); i != attributes.end(); ++i) {
		result |= (0 != SDL_GL_SetAttribute((*i).first, (*i).second));
		if (!result) xeekworx::log << LOGSTAMP << xeekworx::WARNING << "> FAIL : ";
		else xeekworx::log << LOGSTAMP << xeekworx::DEBUG2 << "> OK : ";
		xeekworx::log << std::setw(30) << std::left << sdltools::SDL_GLattrToString((*i).first) << " = ";
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
    // ----------------------------------------------------------------------------
	// STAGE 1, after the window (display) was created
    // ----------------------------------------------------------------------------

	// CREATE THE OPENGL CONTEXT:
	SDL_GLContext new_glcontext = SDL_GL_CreateContext((SDL_Window *)m_display->m_sdlwindow);
	if (new_glcontext == NULL) {
		xeekworx::log << LOGSTAMP << xeekworx::FATAL << "Failed to create OpenGL context" << std::endl;
		return false;
	}
	else {
		m_display->m_glcontext = new_glcontext;
		m_display->make_current();
		xeekworx::log << LOGSTAMP << xeekworx::DEBUG << "OpenGL context created successfully" << std::endl;
	}

	// LOG ACTUAL POST OPENGL CONTEXT CREATION ATTRIBUTES:
	bool result = true;
	std::unordered_map<SDL_GLattr, int> attributes;
	attributes[SDL_GL_CONTEXT_FLAGS] = 0;
	attributes[SDL_GL_CONTEXT_PROFILE_MASK] = 0;
	attributes[SDL_GL_CONTEXT_MAJOR_VERSION] = 0;
	attributes[SDL_GL_CONTEXT_MINOR_VERSION] = 0;
	attributes[SDL_GL_RED_SIZE] = 0;
	attributes[SDL_GL_GREEN_SIZE] = 0;
	attributes[SDL_GL_BLUE_SIZE] = 0;
	attributes[SDL_GL_ALPHA_SIZE] = 0;
	attributes[SDL_GL_DEPTH_SIZE] = 0;
	attributes[SDL_GL_STENCIL_SIZE] = 0;
	attributes[SDL_GL_DOUBLEBUFFER] = 0;
	attributes[SDL_GL_MULTISAMPLEBUFFERS] = 0;
	attributes[SDL_GL_MULTISAMPLESAMPLES] = 0;
	//attributes[SDL_GL_SHARE_WITH_CURRENT_CONTEXT] = 0;
	xeekworx::log << LOGSTAMP << xeekworx::DEBUG << "Actual OpenGL Attributes: " << std::endl;
	for (auto i = attributes.begin(); i != attributes.end(); ++i) {
		result |= (0 != SDL_GL_GetAttribute((*i).first, &(*i).second));
		if (!result) xeekworx::log << LOGSTAMP << xeekworx::WARNING << "> FAIL : ";
		else xeekworx::log << LOGSTAMP << xeekworx::DEBUG2 << "> OK : ";
		xeekworx::log << std::setw(30) << std::left << sdltools::SDL_GLattrToString((*i).first) << " = ";
		switch ((*i).first) {
		case SDL_GL_CONTEXT_FLAGS: xeekworx::log << sdltools::SDL_GLcontextFlagToString((*i).second); break;
		case SDL_GL_CONTEXT_PROFILE_MASK: xeekworx::log << sdltools::SDL_GLprofileToString((*i).second); break;
		case SDL_GL_CONTEXT_RELEASE_BEHAVIOR: xeekworx::log << sdltools::SDL_GLcontextReleaseFlagToString((*i).second); break;
		default: xeekworx::log << (*i).second; break;
		}
		xeekworx::log << std::endl;
	}

	// INITIALIZE GLAD:
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		xeekworx::log << LOGSTAMP << xeekworx::FATAL << "Failed to initialize GLAD" << std::endl;
		SDL_GL_DeleteContext(m_display->m_glcontext);
		m_display->m_glcontext = nullptr;
		return false;
	}
	else {
		xeekworx::log << LOGSTAMP << xeekworx::DEBUG << "GLAD initialized successfully" << std::endl;
	}

	// LOG OPENGL VERSION, VENDOR (IMPLEMENTATION), RENDERER, GLSL, ETC.:
	xeekworx::log << LOGSTAMP << xeekworx::NOTICE << std::setw(34) << std::left << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;
	xeekworx::log << LOGSTAMP << xeekworx::NOTICE << std::setw(34) << std::left << "OpenGL Shading Language Version: " << (char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	xeekworx::log << LOGSTAMP << xeekworx::NOTICE << std::setw(34) << std::left << "OpenGL Vendor:" << (char *)glGetString(GL_VENDOR) << std::endl;
	xeekworx::log << LOGSTAMP << xeekworx::NOTICE << std::setw(34) << std::left << "OpenGL Renderer:" << (char *)glGetString(GL_RENDERER) << std::endl;

	// LOG OPENGL PARAMETERS:
	struct gl_param {
		std::string name; GLenum param; unsigned num_values; bool integer;
		operator GLenum() const { return param; }
	};
	std::vector<gl_param> parameters;
	parameters.push_back(gl_param{ "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS", GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, 1, true });
	parameters.push_back(gl_param{ "GL_MAX_CUBE_MAP_TEXTURE_SIZE", GL_MAX_CUBE_MAP_TEXTURE_SIZE, 1, true });
	parameters.push_back(gl_param{ "GL_MAX_DRAW_BUFFERS", GL_MAX_DRAW_BUFFERS, 1, true });
	parameters.push_back(gl_param{ "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS", GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, 1, true });
	parameters.push_back(gl_param{ "GL_MAX_TEXTURE_IMAGE_UNITS", GL_MAX_TEXTURE_IMAGE_UNITS, 1, true });
	parameters.push_back(gl_param{ "GL_MAX_TEXTURE_SIZE", GL_MAX_TEXTURE_SIZE, 1, true });
	//parameters.push_back(gl_param { "GL_MAX_VARYING_FLOATS", GL_MAX_VARYING_FLOATS, 1, true });
	parameters.push_back(gl_param{ "GL_MAX_VERTEX_ATTRIBS", GL_MAX_VERTEX_ATTRIBS, 1, true });
	parameters.push_back(gl_param{ "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS", GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, 1, true });
	parameters.push_back(gl_param{ "GL_MAX_VERTEX_UNIFORM_COMPONENTS", GL_MAX_VERTEX_UNIFORM_COMPONENTS, 1, true });
	parameters.push_back(gl_param{ "GL_MAX_VIEWPORT_DIMS", GL_MAX_VIEWPORT_DIMS, 2, true });
	parameters.push_back(gl_param{ "GL_STEREO", GL_STEREO, 1, false });
	parameters.push_back(gl_param{ "GL_DOUBLEBUFFER", GL_DOUBLEBUFFER, 1, false });
	//parameters.push_back(gl_param { "GL_MAX_LIGHTS", GL_MAX_LIGHTS, 1, true }); // For GL 4+
	parameters.push_back(gl_param{ "GL_VIEWPORT", GL_VIEWPORT, 4, true });

	xeekworx::log << LOGSTAMP << xeekworx::DEBUG2 << "OpenGL Context Parameters: " << std::endl;
	for (auto i = parameters.begin(); i != parameters.end(); ++i) {
		xeekworx::log << LOGSTAMP << xeekworx::DEBUG2 << "> " << std::setw(35) << std::left << (*i).name << " = ";
		std::vector<GLint> values((*i).num_values);
		glGetIntegerv((*i).param, values.data());
		if (glGetError() != GL_NO_ERROR) xeekworx::log << "ERROR";
		else {
			for (auto v = values.begin(); v != values.end(); v++) {
				if ((*i).num_values > 1 && v != values.begin()) xeekworx::log << ", ";
				if ((*i).integer) xeekworx::log << (*v);
				else xeekworx::log << ((*v) ? "TRUE" : "FALSE");
			}
		}
		xeekworx::log << std::endl;
	}

	// LOG OPENGL ERRORS:
	check_glerrors();

	// CONFIGURE VSYNC:
	if (SDL_GL_SetSwapInterval(gfx_setup.vsync ? 1 : 0) < 0) {
		xeekworx::log << LOGSTAMP << xeekworx::WARNING << "Unable to " << (gfx_setup.vsync ? "enable" : "disable") << " VSync. SDL Error: \"" << SDL_GetError() << "\"" << std::endl;
	}
	else xeekworx::log << LOGSTAMP << xeekworx::DEBUG2 << (gfx_setup.vsync ? "Enabled Vertical Sync" : "Disabled Vertical Sync") << std::endl;


	g = std::make_unique<graphics2d>();

	return result;
}

void application::shutdown_apis()
{
	// UNINITIALIZE SDL:
	SDL_Quit();
	xeekworx::log << LOGSTAMP << xeekworx::DEBUG << "SDL Shutdown" << std::endl;
}

bool application::check_glerrors(bool log)
{
	// LOG OPENGL ERRORS:
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		if (log) {
			xeekworx::log << LOGSTAMP << xeekworx::WARNING << "OpenGL error in queue: \"" << gltools::GL_ErrorToString(err) << "\"" << std::endl;
		}
		return true;
	}
	return false;
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

		// Present:
		m_display->present();
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
	g->begin();
	g->clear(graphics2d::black);
	g->test();
	g->end();
}