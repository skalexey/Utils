#pragma once

#include <string>

namespace utils
{
	// Parses a lunch argument for a particular option and returns its value
	// If the value is not specified then empty string is returned ("")
	struct opt_val
	{
		std::string val;
		opt_val(bool valid, const std::string& val) : m_valid(valid), val(val) {}
		inline operator bool() { return m_valid; }
		
	protected:
		bool m_valid = false;
	};

	opt_val get_option_value(const std::string& arg, const std::string& opt)
	{
		auto p = arg.find(opt);
		if (p != std::string::npos)
		{
			p = arg.find_first_of("=", p);
			if (p != std::string::npos)
			{
				if (p == arg.size() - 1)
					return opt_val( true, "" );
				else
					return opt_val(true, arg.substr(p + 1));
			}
		}
		return {false, ""};
	}
}