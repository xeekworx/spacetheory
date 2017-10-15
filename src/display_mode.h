#pragma once
#include "size.h"

namespace spacetheory {

	struct display_mode {
		size res;
		int refresh_rate = 0;
		uint32_t pixel_format = 0;
	};

}