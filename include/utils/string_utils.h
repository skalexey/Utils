#pragma once

#include <string>
#include <vector>

namespace utils
{
	int count_substring(const std::string& where, const std::string& what);
	std::string str_tolower(std::string s);
	std::string format_str(const char* fmt, ...);
	std::vector<std::string_view> split(const std::string_view& str, const std::string_view& delim);
	std::vector<std::string_view> split_of(const std::string_view& str, const std::string_view& delims);
}
