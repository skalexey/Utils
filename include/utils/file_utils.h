#pragma once

#include <chrono>
#include <fstream>
#include <vector>
#include <functional>
#include <utils/filesystem.h>

namespace
{
	template <typename Container_t>
	inline Container_t file_contents_impl(const std::string& fpath)
	{
		std::ifstream f(fpath);
		return Container_t((std::istreambuf_iterator<char>(f)),
			(std::istreambuf_iterator<char>()));
	}
}

namespace utils
{
	namespace file
	{
		const std::string& last_error();
		int last_errcode();
#ifdef FILESYSTEM_SUPPORTED
		bool exists(const fs::path& fpath);
		bool dir_exists(const fs::path& path);
		int copy(const fs::path& from_path, const fs::path& to_path, bool safe = false);
		int move(const fs::path& from_path, const fs::path& to_path, bool safe = false);
		int remove_last_line(const fs::path& fpath);
		bool remove(const fs::path& fpath);
		std::chrono::system_clock::time_point modif_time(const fs::path& fpath);
		template <typename T = std::string>
		T contents(const fs::path& fpath) {
			return file_contents_impl<T>(fpath.string());
		}
		std::vector<std::string> lines(const fs::path& path);
		bool same(const fs::path& f1, const fs::path& f2);
		int create(const fs::path& path);
		bool is_file_path(const fs::path& path);
		fs::path temp_directory_path();
		bool is_directory(const fs::path& path);
#else
		bool exists(const std::string& fpath);
		int copy(const std::string& from_path, const std::string& to_path, bool safe = false);
		int move(const std::string& from_path, const std::string& to_path, bool safe = false);
		int remove_last_line(const std::string& fpath);
		bool remove(const std::string& fpath);
		std::chrono::system_clock::time_point modif_time(const std::string& fpath);
		template <typename T = std::string>
		T contents(const std::string& fpath) {
			return file_contents_impl<T>(fpath);
		}
		std::vector<std::string> lines(const std::string& path);
		bool same(const std::string& f1, const std::string& f2);
		int create(const std::string& path);
		std::string temp_directory_path();
		bool is_directory(const std::string& path);
#endif

		int remove_last_line_f(std::ifstream& f);
	}
}