#include <sstream>
#include <ios>
#include <algorithm>
#include <iterator>
#include <string>
#include <string_view>
#ifdef __cpp_lib_format
#include <format>
#endif
#if defined(__cpp_lib_ranges)
#include <ranges>
#endif
#include <utils/string_utils.h>

#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */
#define FIRSTH 37 /* also prime */

namespace utils
{
	int count_substring(const std::string& where, const std::string& what)
	{
		int cnt = 0;
		for (std::size_t pos = 0; pos != std::string::npos; cnt++)
			pos = where.find(what, pos) + 1;
		return cnt;
	}

	std::string str_tolower(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(),
			[](unsigned char c) { return std::tolower(c); } // correct
		);
		return s;
	}

	std::string str_toupper(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(),
			[](unsigned char c) { return std::toupper(c); } // correct
		);
		return s;
	}

	std::string format_str(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		static char buf[10512];
		vsnprintf(buf, 10512, fmt, args);
		va_end(args);
		return buf;
	}

	std::vector<std::string_view> split(
		const std::string_view& str
		, const std::string_view& delim
	)
	{
		std::vector<std::string_view> ret;
// TODO: support ranges version
//#if defined(__cpp_lib_ranges)
//		auto v = std::views::split(str, delim);
//		std::copy(v.begin(), v.end(), std::back_inserter(ret));
//#else
		std::size_t p = std::string::npos, cur = std::string::npos;
		do
		{
			cur = ++p;
			p = str.find(delim, p);
			ret.push_back(str.substr(cur, p - cur));
		} while (p != std::string::npos);
//#endif
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

	bool to_bool(std::string str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		std::istringstream is(str);
		bool b;
		is >> std::boolalpha >> b;
		return b;
	}

	bool parse_bool(std::string str)
	{
		return to_bool(str);
	}

	namespace string
	{
		unsigned hash(const std::string& s)
		{
			unsigned h = FIRSTH;
			for (auto it = s.begin(); it != s.end(); ++it)
				h = (h * A) ^ (*it * B);
			return h; // or return h % C;
		}
	}
}
