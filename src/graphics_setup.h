#pragma once

namespace spacetheory {

	struct graphics_setup {
		bool vsync = false;
		bool msaa = false; // multisample antialiasing
		int msaa_samples = 2;
	};

}