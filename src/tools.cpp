#include "tools.h"
#include <sstream>
#include <vector>

using namespace spacetheory;

std::string spacetheory::tools::friendly_duration(const clock::time_point& start, const clock::time_point& end, const bool abbreviate)
{
	std::stringstream result;

	using day_t = std::chrono::duration<long, std::ratio<3600 * 24>>;

	auto dur = end - start;
	auto d = std::chrono::duration_cast<day_t>(dur);
	auto h = std::chrono::duration_cast<std::chrono::hours>(dur -= d);
	auto m = std::chrono::duration_cast<std::chrono::minutes>(dur -= h);
	auto s = std::chrono::duration_cast<std::chrono::seconds>(dur -= m);
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur -= s);

	struct tmp { long long count; std::string name; };
	std::vector<tmp> dur_vec;

	dur_vec.push_back({ d.count(), abbreviate ? "day" : "day" });
	dur_vec.push_back({ h.count(), abbreviate ? "hr" : "hour" });
	dur_vec.push_back({ m.count(), abbreviate ? "min" : "minute" });
	dur_vec.push_back({ s.count(), abbreviate ? "sec" : "second" });
	dur_vec.push_back({ ms.count(), abbreviate ? "ms" : "millisecond" });

	bool first = true;
	for (const auto &x : dur_vec) {
		if (x.count <= 0) continue;
		if (!first) { // Only put a separator in front of other names
			result << " ";
		}
		result << x.count << " " << x.name;
		// Name plurality:
		if ((!abbreviate || x.name == "day") && x.count > 1) result << "s";
		first = false;
	}

	return result.str();
}