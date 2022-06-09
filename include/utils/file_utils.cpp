#include <fstream>
#include <cstdio>
#include <string>
//#include <filesystem>
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

	int file_remove_last_line(const std::string& fpath)
	{
		std::ifstream f(fpath);
		return file_remove_last_line_f(f);
	}

	int file_remove_last_line_f(std::ifstream& f)
	{
		// TODO: implement
		if (f.is_open())
		{
			auto cur_pos = f.tellg();
			f.seekg(0, std::ios::beg);
			std::string line;
			std::size_t last_line_begin = 0;
			while (!f.eof())
			{
				line.clear();
				last_line_begin = f.tellg();
				std::getline(f, line);
				//std::cout << "getline: '" << line << "'\n";
			}
			auto fsize = f.tellg();
			//std::filesystem::resize_file("", 64 * 1024);
			f.seekg(cur_pos, std::ios::beg);
		}
		
		return 0;
	}
}