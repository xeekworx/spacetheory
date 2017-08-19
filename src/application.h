#pragma once
#include <string>
#include <vector>
#include <memory>
#include "display.h"

namespace spacetheory {

	class application
	{
	public:
		application();
		virtual ~application();

		int run(int argc, char *argv[]);
		void shutdown();

		static application * app() { return s_app; }
		display * display() { return m_display; }

	protected:
		virtual bool on_start(const std::vector<std::string>& args, std::shared_ptr<display::setup> display_setup) = 0;
		virtual void on_frame();

	private:
		static spacetheory::application * s_app;
		spacetheory::display * m_display = nullptr;
		bool m_should_quit = false;

		bool create_display(std::shared_ptr<display::setup> display_setup);
		bool setup_apis();
		void close_apis();

		void game_loop();
		bool event_loop();
	};

}