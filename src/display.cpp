#include "display.h"
#include "error.h"
#include "logger.h"
#include <SDL.h>

using namespace spacetheory;

display::display(const display_setup& setup) : m_sdlwindow(nullptr), m_glcontext(nullptr)
{
	// LOG BOUNDS:
	xeekworx::log << LOGSTAMP << xeekworx::logtype::DEBUG << "Game window setup ..." << std::endl;
	xeekworx::log << LOGSTAMP << xeekworx::logtype::DEBUG << "Bounds: "
		<< setup.bounds.x << ", " << setup.bounds.y << ", "
		<< setup.bounds.w << " x " << setup.bounds.h
		<< std::endl;

	// DETERMINE THE GAME WINDOW POSITION:
	xeekworx::log << LOGSTAMP << xeekworx::logtype::DEBUG << "Positioning: ";
	int x = setup.bounds.x, y = setup.bounds.y;
	switch (setup.positioning) {
	case window_positioning::centered:
		x = y = SDL_WINDOWPOS_CENTERED_DISPLAY(setup.on_screen);
		xeekworx::log << "Centered (on screen " << setup.on_screen << ")" << std::endl;
		break;
	case window_positioning::undefined:
		x = y = SDL_WINDOWPOS_UNDEFINED_DISPLAY(setup.on_screen);
		xeekworx::log << "Undefined (on screen " << setup.on_screen << ")" << std::endl;
		break;
	case window_positioning::coordinates:
		xeekworx::log << "Using Coordinates" << std::endl;
		break;
	default:
		xeekworx::log << "Unknown" << std::endl;
		break;
	}

	// FULL-SCREEN MODE:
	xeekworx::log << LOGSTAMP << xeekworx::logtype::DEBUG << "Window Mode: ";
	uint32_t wndflags = SDL_WINDOW_OPENGL;
	switch (setup.mode) {
	case window_mode::native_fullscreen:
		wndflags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		xeekworx::log << "Fullscreen Desktop" << std::endl;
		break;
	case window_mode::fullscreen:
		wndflags |= SDL_WINDOW_FULLSCREEN;
		xeekworx::log << "Fullscreen" << std::endl;
		break;
	case window_mode::windowed:
		xeekworx::log << "Windowed" << std::endl;
		break;
	default:
		xeekworx::log << "Unknown" << std::endl;
		break;
	}

	// CREATE THE GAME WINDOW:
	xeekworx::log << LOGSTAMP << xeekworx::logtype::DEBUG << "Creating game window named '" << setup.name << "'" << std::endl;
	m_sdlwindow = SDL_CreateWindow(
		setup.name.c_str(), 
		x, y, 
		setup.bounds.w, setup.bounds.h,
		wndflags
	);
	if (m_sdlwindow == NULL) {
		const std::string msg = "SDL_CreateWindow() failed with error: " + std::string(SDL_GetError());
		xeekworx::log << LOGSTAMP << xeekworx::logtype::ERR << msg << std::endl;
		throw(spacetheory::error(msg));
	}

	xeekworx::log << LOGSTAMP << xeekworx::logtype::DEBUG2 << "Game window constructed" << std::endl;
}

display::~display()
{
	if (m_glcontext) {
		SDL_GL_DeleteContext((SDL_GLContext)m_glcontext);
		m_glcontext = nullptr;
	}
	if (m_sdlwindow) {
		SDL_DestroyWindow((SDL_Window*)m_sdlwindow);
		m_sdlwindow = nullptr;
	}

	xeekworx::log << LOGSTAMP << xeekworx::logtype::DEBUG2 << "Game window destroyed" << std::endl;
}
