#include "demo_application.h"

demo_application::demo_application()
{
	xeekworx::logger::config logconfig = xeekworx::logger::default_config();
	logconfig.output_to_file = true;
	applog.enable(logconfig);

	applog << LOGSTAMP << xeekworx::DEBUG << "Demo application constructed" << std::endl;
}

demo_application::~demo_application()
{
	applog << LOGSTAMP << xeekworx::DEBUG << "Demo application destructed" << std::endl;
}

bool demo_application::on_start(const std::vector<std::string>& args, spacetheory::display_setup& disp_setup, spacetheory::graphics_setup& gfx_setup)
{
	applog << LOGSTAMP << xeekworx::DEBUG << "Starting..." << std::endl;

	disp_setup.on_screen = 0;
	disp_setup.mode = spacetheory::window_mode::windowed;
	disp_setup.name = "Spacetheory Demo Application";
    gfx_setup.vsync = false;

	return true;
}
