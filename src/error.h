#pragma once
#include <stdexcept>

namespace spacetheory {

	class error : public std::runtime_error
	{
		// May extend the capability of this in the future, but to avoid the
		// mass changes needed I'm going to go ahead and make use of this 
		// everywhere.

	public:

		explicit error(const std::string& what_arg) : runtime_error(what_arg) {}
		explicit error(const char* what_arg) : runtime_error(what_arg) {}
	};

}