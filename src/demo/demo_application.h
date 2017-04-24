#pragma once
#include <spacetheory.h>

class demo_application :
	public spacetheory::application
{
public:
	demo_application();
	virtual ~demo_application();

protected:
	bool on_start(const std::vector<std::string>& args, int& exitcode) override;
};

