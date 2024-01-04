#include <cassert>
#include <sstream>
#include <ios>
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
	std::string_view to_str(const char* str)
	{
		if (str == nullptr)
			return {"nullptr"};
		else
			return {str};
	}

	int count_substring(const std::string& where, const std::string& what)
	{
		int cnt = 0;
		for (std::size_t pos = 0; pos != std::string::npos; cnt++)
			pos = where.find(what, pos) + 1;
		return cnt;
	}

	std::string format_str(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		int buf_size = 0, n = 0, cnt = 0, trials = 3; // 1 for check, 2 for the actual formatting and 3 for spare
		std::string buf;
		while ((n = vsnprintf(buf.data(), buf_size, fmt, args)) > buf_size && cnt++ < trials)
		{
			buf = std::string(buf_size = n + 1, 0);
			va_end(args);
			va_start(args, fmt);
		}
		assert(n <= buf.size());
		buf.resize(n);
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

		// trim from start (in place)
		void ltrim(std::string &s)
		{
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
				return !std::isspace(ch);
			}));
		}

		// trim from end (in place)
		void rtrim(std::string &s)
		{
			s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
				return !std::isspace(ch);
			}).base(), s.end());
		}

		// trim from both ends (in place)
		void trim(std::string &s)
		{
			rtrim(s);
			ltrim(s);
		}

	}
}
