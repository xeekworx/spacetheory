#include "demo_application.h"

demo_application::demo_application()
{
	xeekworx::logger::config logconfig = xeekworx::logger::default_config();
	logconfig.output_to_file = true;
	applog.enable(logconfig);

	applog << LOGSTAMP << xeekworx::DEBUG << "Demo Application Constructed" << std::endl;
}

demo_application::~demo_application()
{
	applog << LOGSTAMP << xeekworx::DEBUG << "Demo Application Destructed" << std::endl;
}

bool demo_application::on_start(const std::vector<std::string>& args, std::shared_ptr<spacetheory::display::setup> display_setup)
{
	applog << LOGSTAMP << xeekworx::DEBUG << "Starting." << std::endl;

	display_setup->name = "Spacetheory Demo Application";

	return true;
}
