#pragma once
#include <string>
#include <chrono>

namespace spacetheory {
	namespace tools {
		using clock = std::chrono::high_resolution_clock;

		std::wstring friendly_duration(const clock::time_point& start, const clock::time_point& end, const bool abbreviate = true);
	}
}