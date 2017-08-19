#include "version.h"
#include <time.h>

using namespace spacetheory;

spacetheory::version spacetheory::get_version()
{
	return { SPACETHEORY_PRODUCTVERSION };
}

std::string spacetheory::get_builddate()
{
	// Get the build date in Unix Time:
	time_t unix_time = SPACETHEORY_BUILDDATE;

	// Convert the Unix Time to Local Time:
	tm local_time = {};
	localtime_s(&local_time, &unix_time);

	// Convert the Local Time to a string:
	char tmp[48] = {};
	asctime_s(tmp, &local_time);
	
	// Remove newline (damn you asctime!):
	std::string result = tmp;
	result.pop_back();

	return result;
}