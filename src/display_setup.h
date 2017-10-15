#pragma once
#include <string>
#include "rectangle.h"
#include "display_mode.h"

namespace spacetheory {

	enum class window_mode {
		windowed, fullscreen, native_fullscreen
	};

	enum class window_positioning {
		coordinates, centered, undefined
	};

	struct display_setup {
		std::string name;
		window_positioning positioning = window_positioning::undefined;
		int on_screen = 0;
		rectangle bounds = rectangle(0, 0, 1280, 720);
		window_mode mode = window_mode::windowed;
	};

}