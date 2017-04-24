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
		virtual bool on_start(const std::vector<std::string>& args, int& exitcode) = 0;

	private:
		void run();
	};

}