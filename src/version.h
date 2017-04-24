#pragma once

#include "version_defs.h"

namespace spacetheory {
	struct version { unsigned int major, minor, revision, build;  };

	version get_version();
}
