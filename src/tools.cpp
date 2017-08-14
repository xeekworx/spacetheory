#include "tools.h"
#include <sstream>
#include <vector>

using namespace spacetheory;

std::wstring spacetheory::tools::friendly_duration(const clock::time_point& start, const clock::time_point& end, const bool abbreviate)
{
	std::wstringstream result;

	using day_t = std::chrono::duration<long, std::ratio<3600 * 24>>;

	auto dur = end - start;
	auto d = std::chrono::duration_cast<day_t>(dur);
	auto h = std::chrono::duration_cast<std::chrono::hours>(dur -= d);
	auto m = std::chrono::duration_cast<std::chrono::minutes>(dur -= h);
	auto s = std::chrono::duration_cast<std::chrono::seconds>(dur -= m);
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur -= s);

	struct tmp { long long count; std::wstring name; };
	std::vector<tmp> dur_vec;

	dur_vec.push_back({ d.count(), abbreviate ? L"day" : L"day" });
	dur_vec.push_back({ h.count(), abbreviate ? L"hr" : L"hour" });
	dur_vec.push_back({ m.count(), abbreviate ? L"min" : L"minute" });
	dur_vec.push_back({ s.count(), abbreviate ? L"sec" : L"second" });
	dur_vec.push_back({ ms.count(), abbreviate ? L"ms" : L"millisecond" });

	bool first = true;
	for (const auto &x : dur_vec) {
		if (x.count <= 0) continue;
		if (!first) { // Only put a separator in front of other names
			result << L" ";
		}
		result << x.count << L" " << x.name;
		// Name plurality:
		if ((!abbreviate || x.name == L"day") && x.count > 1) result << L"s";
		first = false;
	}

	return result.str();
}