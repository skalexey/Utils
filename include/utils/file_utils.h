#pragma once

#include <fstream>

namespace utils
{
	int file_exists(const std::string& fpath)
	{
		return std::ifstream(fpath.c_str()).good();
	}
}