#pragma once

#include <vector>
#include <string>

#define _for(x, s, e) for (int x = s; x < e; x++)
#define _fore(x, s, e) for (int x = s; x <= e; x++)

namespace oop {

	std::vector<std::string> split(const std::string& str, const std::string& del);

	std::string join(const std::vector<std::string>& vec, const std::string& del);

}