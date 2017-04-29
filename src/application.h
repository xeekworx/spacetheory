#pragma once
#include <string>
#include <vector>

namespace spacetheory {

	class application
	{
	public:
		application();
		virtual ~application();

		int run(int argc, char *argv[]);

	protected:
		virtual bool on_start(const std::vector<std::string>& args, int& exitcode) = 0;

	private:
		static spacetheory::application * s_app;

		bool setup_apis();
		void close_apis();
		void game_loop();
	};

}