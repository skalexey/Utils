#pragma once

#include <chrono>
#include <fstream>
#ifdef __cpp_lib_filesystem
#include <filesystem>
#endif

namespace utils
{
	int file_exists(const std::string& fpath);
#ifdef __cpp_lib_filesystem
	int copy_file(const std::filesystem::path& from_path, const std::filesystem::path& to_path, bool safe = false);
	int move_file(const std::filesystem::path& from_path, const std::filesystem::path& to_path, bool safe = false);
	int file_remove_last_line(const std::filesystem::path& fpath);
	bool remove_file(const std::filesystem::path& fpath);
	std::chrono::system_clock::time_point file_modif_time(const std::filesystem::path& fpath);
	std::string file_contents(const std::filesystem::path& fpath);
#else
	int copy_file(const std::string& from_path, const std::string& to_path, bool safe = false);
	int move_file(const std::string& from_path, const std::string& to_path, bool safe = false);
	int file_remove_last_line(const std::string& fpath);
	bool remove_file(const std::string& fpath);
	std::chrono::system_clock::time_point file_modif_time(const std::string& fpath);
	std::string file_contents(const std::string& fpath);
#endif
	
	int file_remove_last_line_f(std::ifstream& f);
}