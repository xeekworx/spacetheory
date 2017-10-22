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
		void * m_sdlwindow;

		display(const display_setup& setup);

	public:
		~display();
	};

}