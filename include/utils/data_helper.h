#pragma once

#include <string>
#include <functional>

namespace utils
{
	namespace data 
	{
		using id_check_t = std::function<bool(const std::string&)>;
		std::string free_id(const std::string& desired, const id_check_t& pred);
	}
}
