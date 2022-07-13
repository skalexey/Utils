// string_convertible.h

#pragma once

#include <string>

namespace interfaces
{
	class string_convertible
	{
	public:
		virtual std::string to_string() const = 0;
		inline operator std::string() const {
			return to_string();
		}
	};

	std::ostream& operator << (std::ostream& os, const string_convertible& p);
}

