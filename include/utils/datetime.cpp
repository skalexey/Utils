//
//  datetime.cpp
//
//  Created by skorokhodov on 2022/06/05.
//

#include <string>
#include <chrono>
#include <stdio.h>
#include <ctime>
#include "datetime.h"

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
}
