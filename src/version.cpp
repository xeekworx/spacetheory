#include "version.h"

using namespace spacetheory;

spacetheory::version spacetheory::get_version()
{
	return { SPACETHEORY_PRODUCTVERSION };
}