#pragma once

#include <fstream>

namespace utils
{
	int file_exists(const std::string& fpath);
	int copy_file(const std::string& from_path, const std::string& to_path, bool safe = false);
	int move_file(const std::string& from_path, const std::string& to_path, bool safe = false);
	int file_remove_last_line(const std::string& fpath);
	int file_remove_last_line_f(std::ifstream& f);
}