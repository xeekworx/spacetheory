#pragma once
#include <string>
#include <vector>

namespace spacetheory {

	class application
	{
	public:
		application();
		virtual ~application();

		int start(int argc, char *argv[]);

	protected:
		virtual int on_start(const std::vector<std::string>& args) = 0;
	};

}