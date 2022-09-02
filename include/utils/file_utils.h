#pragma once

#include <chrono>
#include <fstream>
#include <functional>
#include "filesystem.h"

namespace utils
{
	namespace file
	{
		const std::string& last_error();
#ifdef FILESYSTEM_SUPPORTED
		bool exists(const std::filesystem::path& fpath);
		bool dir_exists(const std::filesystem::path& path);
		int copy_file(const std::filesystem::path& from_path, const std::filesystem::path& to_path, bool safe = false);
		int move_file(const std::filesystem::path& from_path, const std::filesystem::path& to_path, bool safe = false);
		int remove_last_line(const std::filesystem::path& fpath);
		bool remove_file(const std::filesystem::path& fpath);
		std::chrono::system_clock::time_point modif_time(const std::filesystem::path& fpath);
		std::string contents(const std::filesystem::path& fpath);
		bool same(const std::filesystem::path& f1, const std::filesystem::path& f2);
#else
		bool exists(const std::string& fpath);
		int copy_file(const std::string& from_path, const std::string& to_path, bool safe = false);
		int move_file(const std::string& from_path, const std::string& to_path, bool safe = false);
		int remove_last_line(const std::string& fpath);
		bool remove_file(const std::string& fpath);
		std::chrono::system_clock::time_point modif_time(const std::string& fpath);
		std::string contents(const std::string& fpath);
		bool same(const std::string& f1, const std::string& f2);
#endif

		int remove_last_line_f(std::ifstream& f);
	}
}