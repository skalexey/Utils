#pragma once

#include <limits>
#include "data_helper.h"
#include "string_utils.h"

namespace utils
{
	namespace data 
	{
		std::string free_id(const std::string& desired, const id_check_t& pred) {
			auto new_id = desired.empty() ? "new_prop" : desired;
			bool success = true;
			if (pred(new_id))
			{
				std::string tpl = new_id;
				for (int i = 1; i < std::numeric_limits<short>::max(); i++)
					if ((success = !pred(new_id = utils::format_str("%s %d", tpl.c_str(), i))))
						break;
			}
			if (success)
				return new_id;
			return "";
		}
	}
}
