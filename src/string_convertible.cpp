// string_convertible.cpp

#include <ostream>
#include <utils/log.h>
#include <interfaces/string_convertible.h>

LOG_TITLE("string_convertible");

namespace interfaces
{
	std::ostream& operator << (std::ostream& os, const string_convertible& p)
	{
		os << p.to_string();
		return os;
	}
}