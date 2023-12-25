#pragma once

#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>

namespace utils
{
	using namespace std;

	string url_encode(const string &value)
	{
		ostringstream escaped;
		escaped.fill('0');
		escaped << hex;
		for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i)
		{
			string::value_type c = (*i);
			// Keep alphanumeric and other accepted characters intact
			if (isdigit(c) || c == '-' || c == '_' || c == '.' || c == '~')
			{
				escaped << uppercase;
				escaped << '%' << setw(2) << int((unsigned char)c);
				escaped << nouppercase;
			}
			else
			{
				escaped << c;
			}
		}
		return escaped.str();
	}
}