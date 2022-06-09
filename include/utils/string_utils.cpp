#include <algorithm>
#include <iterator>
#include <cstdarg>
#include <string>
#include <cstdio>
#include <string_view>
#if __cplusplus > 201703L
#include <ranges>
#endif
#include "string_utils.h"

namespace utils
{
	int count_substring(const std::string& where, const std::string& what)
	{
		int cnt = 0;
		for (std::size_t pos = 0; pos != std::string::npos; cnt++)
			pos = where.find(what, pos) + 1;
		return cnt;
	}

	std::string str_tolower(std::string s) {
		std::transform(s.begin(), s.end(), s.begin(),
			[](unsigned char c) { return std::tolower(c); } // correct
		);
		return s;
	}

	std::string format_str(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		char buf[512];
		vsnprintf(buf, 512, fmt, args);
		va_end(args);
		return buf;
	}

	std::vector<std::string_view> split(
		const std::string_view& str
		, const std::string_view& delim
	)
	{
		std::vector<std::string_view> ret;
#if __cplusplus > 201703L
		auto v = std::views::split(parts, delim);
		std::copy(v.begin(), v.end(), std::back_inserter(sub));
#else
		std::size_t p = std::string::npos, cur = std::string::npos;
		do
		{
			cur = ++p;
			p = str.find(delim, p);
			ret.push_back(str.substr(cur, p - cur));
		} while (p != std::string::npos);
#endif
		return ret;
	}

	std::vector<std::string_view> split_of(
		const std::string_view& str
		, const std::string_view& delims
	)
	{
		std::size_t p = std::string::npos, cur = std::string::npos;
		std::vector<std::string_view> ret;
		do
		{
			cur = ++p;
			p = str.find_first_of(delims, p);
			ret.push_back(str.substr(cur, p - cur));
		} while (p != std::string::npos);
		return ret;
	}
}
