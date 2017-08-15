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
		void shutdown();

	protected:
		virtual bool on_start(const std::vector<std::string>& args) = 0;
		virtual void on_frame();

	private:
		static spacetheory::application * s_app;
		bool m_should_quit;

		bool setup_apis();
		void close_apis();
		void game_loop();
		bool event_loop();
	};

}