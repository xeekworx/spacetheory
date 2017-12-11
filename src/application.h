#pragma once
#include <string>
#include <vector>
#include <memory>
#include "display.h"
#include "graphics_setup.h"
#include "graphics2d.h"

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
		virtual bool on_start(const std::vector<std::string>& args, display_setup& disp_setup, graphics_setup& gfx_setup) = 0;
		virtual void on_frame();

	private:
		static spacetheory::application * s_app;
		spacetheory::display * m_display = nullptr;
		bool m_should_quit = false;
		std::unique_ptr<graphics2d> g;

		bool create_display(const display_setup& disp_setup);
		bool setup_apis1(); // stage 1, before any apis have initialized
		bool setup_apis2(const graphics_setup& gfx_setup); // stage 2, before the window (display) is created
		bool setup_apis3(const graphics_setup& gfx_setup); // stage 3, after the window (display) was created
		void shutdown_apis();

		static bool check_glerrors(bool log = true);

		void game_loop();
		bool event_loop();
	};

}