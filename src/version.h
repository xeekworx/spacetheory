#pragma once

#include "version_defs.h"
#include <string>

namespace spacetheory {
	struct version { unsigned int major, minor, revision, build;  };

	version get_version();
	std::wstring get_builddate();
}
