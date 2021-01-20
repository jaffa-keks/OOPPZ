#include "oop_lib.h"

namespace oop {

	std::vector<std::string> split(const std::string& str, const std::string& del) {
		std::vector<std::string> result;
		size_t i = 0, j = 0, del_len = del.size();

		while ((j = str.find(del, i)) != std::string::npos) {
			result.push_back(str.substr(i, j - i));
			i = j + del_len;
		}

		result.push_back(str.substr(i, str.size() - i));
		return result;
	}

	std::string join(const std::vector<std::string>& vec, const std::string& del) {
		std::string res = vec[0];
		_for(i, 1, vec.size()) res += " " + vec[i];
		return res;
	}

}