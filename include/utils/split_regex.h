#pragma once

#include <string>
#include <string_view>
#include <boost/algorithm/string/regex.hpp>

namespace utils
{
	template <typename CollectionT>
	void split_regex(CollectionT& result, const std::string& line, const std::string& regex)
	{
#if __cplusplus < 202002L
		static_assert(!std::is_same<CollectionT::value_type, std::string_view>::value
			&& "using std::string_view as an element for output collection passed into boost::algorithm::split_regex is only supported in C++20. "
			"We have no alternative instead of boost for now."
		);
#endif
		boost::algorithm::split_regex(result, line, boost::regex(regex));
	}
}
