#pragma once

#include <cstdarg>
#include <cstdio>
#include <string>
#include <vector>

#define SSTREAM(ex) (std::stringstream() << ex).str()

namespace utils
{
	std::string_view to_str(const char* str);
	int count_substring(const std::string& where, const std::string& what);
	std::string str_tolower(std::string s);
	std::string str_toupper(std::string s);
	std::string format_str(const char* fmt, ...);
	std::vector<std::string_view> split(const std::string_view& str, const std::string_view& delim);
	std::vector<std::string_view> split_of(const std::string_view& str, const std::string_view& delims);
	std::vector<std::string_view> split_repeated_delimeter(const std::string& str, const char delim);
	bool to_bool(std::string str);
	bool parse_bool(std::string str);
	
	// Example: "a\t\t\tb\tc" -> {"a", "b", "c"}
	template <typename CollectionT>
	void split_repeated_delimeter(CollectionT& result, const std::string& line, const char delim)
	{
		using value_type = typename CollectionT::value_type;
		std::size_t tpos = 0, pos = 0;
		while ((tpos = line.find("\t", pos)) != std::string::npos)
		{
			value_type s(&*(line.data() + pos), tpos - pos);
			while (line.find(delim, tpos + 1) - tpos == 1)
				tpos++;
			pos = tpos + 1;
			result.push_back(s);
		}
		if (line.size() - pos != 0)
			result.push_back(value_type(&*(line.data() + pos), line.size() - pos));
	}

	namespace string
	{
		unsigned hash(const std::string& s);
		void trim(std::string& s);
		void rtrim(std::string &s);
		void trim(std::string &s);
	}

}
