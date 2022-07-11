//
//  datetime.h
//
//  Created by skorokhodov on 2022/06/05.
//

#pragma once

#include <string>
#include <chrono>

namespace utils
{
	// Local time is used for every of these functions
	std::string current_datetime(const std::string& format = "");
	std::chrono::system_clock::time_point parse_datetime(const std::string& str, const std::string& fmt);
	std::chrono::system_clock::time_point parse_datetime_http(const std::string& str);
	std::string time_to_string(const std::chrono::system_clock::time_point& tp);
}
