#pragma once

#include <string>

namespace utils
{
	int count_substring(const std::string& where, const std::string& what)
	{
		int cnt = 0;
		for (std::size_t pos = 0; pos != std::string::npos; cnt++)
			pos = where.find(what, pos) + 1;
		return cnt;
	}
}