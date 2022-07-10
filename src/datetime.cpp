//
//  datetime.cpp
//
//  Created by skorokhodov on 2022/06/05.
//

#include <string>
#include <stdio.h>
#include <ctime>
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
#if  __cplusplus >= 202002L // Use C++20 syntax
		ch::system_clock::time_point tp;
		std::stringstream ss(str);
		ss >> ch::parse(fmt, tp);
		return tp;
	#else
		// From:
		// https://stackoverflow.com/questions/21021388/how-to-parse-a-date-string-into-a-c11-stdchrono-time-point-or-similar
		std::tm tm = {};
		std::stringstream ss(str);
		ss >> std::get_time(&tm, fmt.c.str());
		auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
		return tp;
		//std::tm tm = {};
		//strptime("Thu Jan 9 2014 12:35:34", "%a %b %d %Y %H:%M:%S", &tm);
		//auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
#endif
	}

	ch::system_clock::time_point parse_datetime_http(const std::string& str)
	{
		// Fri, 08 Jul 2022 15:47 : 48 GMT
		return parse_datetime(str, "%a, %d %b %C%y %H:%M:%S GMT");
	}
}
