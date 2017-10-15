#pragma once
#include <string>
#include <vector>
#include <memory>
#include "display_setup.h"

namespace spacetheory {

	class application;

	class display
	{
		friend spacetheory::application;
	private:
		std::shared_ptr<display_setup> m_setup;
		void * m_sdlwindow;

		display(std::shared_ptr<display_setup> s);

	public:
		~display();
	};

}