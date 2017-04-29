#include "demo_application.h"

demo_application::demo_application()
{
	xeekworx::logger::config logconfig = xeekworx::logger::default_config();
	logconfig.output_to_file = true;
	applog.enable(logconfig);

	applog << LOGSTAMP << xeekworx::DEBUG << L"Demo Application Constructed" << std::endl;
}

demo_application::~demo_application()
{
	applog << LOGSTAMP << xeekworx::DEBUG << L"Demo Application Destructed" << std::endl;
}

bool demo_application::on_start(const std::vector<std::string>& args, int & exitcode)
{
	applog << LOGSTAMP << xeekworx::DEBUG << L"Starting." << std::endl;
	return true;
}
