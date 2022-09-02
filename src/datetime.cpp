//
//  datetime.cpp
//
//  Created by skorokhodov on 2022/06/05.
//

#if defined __APPLE__ || __cplusplus < 202002L
#include <iomanip>
#include <ctime>
#include <cstdlib>
#endif
#include <sstream>
#include <string>
#include <stdio.h>
#include <iomanip>
#include "datetime.h"

namespace ch = std::chrono;

namespace utils
{
	std::string current_datetime(const std::string& format)
	{
		char buf[1024];
		std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
		time_t tnow = std::chrono::system_clock::to_time_t(now);
		tm *date = std::localtime(&tnow);
		long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		sprintf(buf, format.empty() ? "%02i:%02i:%02i:%03li" : format.c_str(), date->tm_hour, date->tm_min, date->tm_sec, ms % 1000);
		return buf;
	}


	ch::system_clock::time_point parse_datetime(const std::string& str, const std::string& fmt)
	{
#if  __cplusplus >= 202002L && !defined(__APPLE__) && !defined(__GNUC__) // Use C++20 syntax
		ch::system_clock::time_point tp;
		std::stringstream ss(str);
		ss >> ch::parse(fmt, tp);
		return tp;
	#else
		// From:
		// https://stackoverflow.com/questions/21021388/how-to-parse-a-date-string-into-a-c11-stdchrono-time-point-or-similar
		if (fmt.find("GMT") != std::string::npos || fmt.find("UTC") != std::string::npos)
		{	// Parse as UTC time
			auto tz = std::getenv("TZ");
			setenv("TZ", "UTC", 1);
			std::tm tm{};
			std::stringstream ss(str);
			ss >> std::get_time(&tm, fmt.c_str());
			auto t = std::mktime(&tm);
			if (tz)
				setenv("TZ", tz, 1);
			else
				unsetenv("TZ");
			return std::chrono::system_clock::from_time_t(t);
		}
		else
		{
			// Parse as local time
			std::tm tm{};
			auto t = std::mktime(&tm);
			std::stringstream ss(str);
			ss >> std::get_time(&tm, fmt.c_str());
			return std::chrono::system_clock::from_time_t(t);
		}
#endif
	}

	ch::system_clock::time_point parse_datetime_http(const std::string& str)
	{
		// Fri, 08 Jul 2022 15:47 : 48 GMT
		return parse_datetime(str, "%a, %d %b %Y %H:%M:%S GMT");
	}

	std::string time_to_string(const ch::system_clock::time_point& tp, bool ms)
	{
		// TODO: branch for C++20
		using namespace std::chrono;
		std::time_t t = std::chrono::system_clock::to_time_t(tp);
		std::stringstream ss;
		ss << std::ctime(&t);
		if (ms)
		{
			ss << std::ctime(&t) << " : " << std::setw(4) << std::setfill('0')
				<< duration_cast<milliseconds>(
					system_clock::now().time_since_epoch()
				).count() % 1000;
		}
		return ss.str();
	}

	std::string time(bool ms)
	{
		return time_to_string(ch::system_clock::now(), ms);
	}
}

