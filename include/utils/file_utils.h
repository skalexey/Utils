#pragma once

#include <chrono>
#include <fstream>
#include <functional>
#include "filesystem.h"

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
		bool exists(const std::filesystem::path& fpath);
		bool dir_exists(const std::filesystem::path& path);
		int copy_file(const std::filesystem::path& from_path, const std::filesystem::path& to_path, bool safe = false);
		int move_file(const std::filesystem::path& from_path, const std::filesystem::path& to_path, bool safe = false);
		int remove_last_line(const std::filesystem::path& fpath);
		bool remove_file(const std::filesystem::path& fpath);
		std::chrono::system_clock::time_point modif_time(const std::filesystem::path& fpath);
		template <typename T = std::string>
		T contents(const std::filesystem::path& fpath) {
			return file_contents_impl<T>(fpath.string());
		}
		std::vector<std::string> lines(const std::filesystem::path& path);
		bool same(const std::filesystem::path& f1, const std::filesystem::path& f2);
#else
		bool exists(const std::string& fpath);
		int copy_file(const std::string& from_path, const std::string& to_path, bool safe = false);
		int move_file(const std::string& from_path, const std::string& to_path, bool safe = false);
		int remove_last_line(const std::string& fpath);
		bool remove_file(const std::string& fpath);
		std::chrono::system_clock::time_point modif_time(const std::string& fpath);
		template <typename T = std::string>
		T contents(const std::string& fpath) {
			return file_contents_impl<T>(fpath);
		}
		std::vector<std::string> lines(const std::string& path);
		bool same(const std::string& f1, const std::string& f2);
#endif

		int remove_last_line_f(std::ifstream& f);
	}
}