#include "display.h"
#include "error.h"
#include "logger.h"
#include <SDL.h>

using namespace spacetheory;

display::display(std::shared_ptr<display_setup> s) : m_sdlwindow(nullptr)
{
	this->m_setup = s;

	// LOG BOUNDS:
	xeekworx::log << LOGSTAMP << xeekworx::logtype::DEBUG << "Game window setup ..." << std::endl;
	xeekworx::log << LOGSTAMP << xeekworx::logtype::DEBUG << "Bounds: "
		<< m_setup->bounds.x << ", " << m_setup->bounds.y << ", "
		<< m_setup->bounds.w << " x " << m_setup->bounds.h
		<< std::endl;

	// DETERMINE THE GAME WINDOW POSITION:
	xeekworx::log << LOGSTAMP << xeekworx::logtype::DEBUG << "Positioning: ";
	int x = m_setup->bounds.x, y = m_setup->bounds.y;
	switch (m_setup->positioning) {
	case window_positioning::centered:
		x = y = SDL_WINDOWPOS_CENTERED_DISPLAY(m_setup->on_screen);
		xeekworx::log << "Centered (on screen " << m_setup->on_screen << ")" << std::endl;
		break;
	case window_positioning::undefined:
		x = y = SDL_WINDOWPOS_UNDEFINED_DISPLAY(m_setup->on_screen);
		xeekworx::log << "Undefined (on screen " << m_setup->on_screen << ")" << std::endl;
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
	switch (m_setup->mode) {
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
	xeekworx::log << LOGSTAMP << xeekworx::logtype::NOTICE << "Creating game window named '" << m_setup->name << "'" << std::endl;
	m_sdlwindow = SDL_CreateWindow(
		m_setup->name.c_str(), 
		x, y, 
		m_setup->bounds.w, m_setup->bounds.h,
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
	if (m_sdlwindow) {
		SDL_DestroyWindow((SDL_Window*)m_sdlwindow);
		m_sdlwindow = nullptr;
	}

	xeekworx::log << LOGSTAMP << xeekworx::logtype::DEBUG2 << "Game window destroyed" << std::endl;
}
