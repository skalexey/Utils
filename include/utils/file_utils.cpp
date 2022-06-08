#include <fstream>
#include <cstdio>
#include "file_utils.h"

namespace utils
{
	int file_exists(const std::string& fpath)
	{
		return std::ifstream(fpath.c_str()).good();
	}

	int copy_file(const std::string& from_path, const std::string& to_path, bool safe)
	{
		std::ifstream src(from_path, std::ios::binary);

		// Check if the copied file exists
		if (!src.is_open())
			return 1;

		// Check if the target path contains a file in the safe mode
		if (safe)
		{
			std::ifstream dst_check(to_path, std::ios::binary);
			if (src.is_open())
				return 2;
			dst_check.close();
		}

		// Copy the file
    	std::ofstream dst(to_path, std::ios::binary);
    	dst << src.rdbuf();
		dst.close();
		std::ifstream dst_check(to_path, std::ios::binary);
		if (!dst_check.is_open())
			return 3;
		return 0;
	}

	int move_file(const std::string& from_path, const std::string& to_path, bool safe)
	{
		auto ret = copy_file(from_path, to_path, safe);
		if (ret == 0)
			std::remove(from_path.c_str());
		return ret;
	}

}