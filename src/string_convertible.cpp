// string_convertible.cpp

#include <ostream>
#include <utils/Log.h>
#include <interfaces/string_convertible.h>

LOG_PREFIX("[string_convertible]: ");
LOG_POSTFIX("\n");

namespace interfaces
{
	std::ostream& operator << (std::ostream& os, const string_convertible& p)
	{
		os << p.to_string();
		return os;
	}
}