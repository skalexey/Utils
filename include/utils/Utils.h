//
//  Utils.h
//
//  Created by skorokhodov on 11/22/17.
//

#pragma once

#include <string>

namespace Utils
{
	void Log(const char* fmt, ...);
	void Warn(const char* fmt, ...);
	unsigned Hash(const std::string& data);
	std::string FormatStr(const char* fmt, ...);
}
