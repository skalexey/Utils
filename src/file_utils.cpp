#include <type_traits>
#include <fstream>
#include <string>
#include <chrono>
#ifndef FILESYSTEM_SUPPORTED
#include <cstdio>
#endif
#include "file_utils.h"

namespace ch = std::chrono;

namespace
{
	std::string last_error_msg;
}

namespace utils
{
	namespace file
	{
		const std::string& last_error()
		{
			return last_error_msg;
		}

		void set_last_error(const std::string& s)
		{
			last_error_msg = s;
		}

	#ifdef FILESYSTEM_SUPPORTED
		bool same(const fs::path& f1, const fs::path& f2)
	#else
		bool same(const std::string& f1, const std::string& f2)
	#endif
		{
			auto f1_contents = contents(f1);
			auto f2_contents = contents(f2);
			return std::hash<std::string>{}(f1_contents)
				== std::hash<std::string>{}(f2_contents);
		}

		bool exists(const std::string& fpath)
		{
			return std::ifstream(fpath.c_str()).good();
		}

	#ifdef FILESYSTEM_SUPPORTED
		bool exists(const std::filesystem::path& fpath)
		{
			return fs::exists(fpath);
		}

		bool dir_exists(const std::filesystem::path& path)
		{
			return fs::exists(path);
		}

		int copy_file(const fs::path& from_path, const fs::path& to_path, bool safe)
		{
			if (!fs::exists(from_path))
				return 1;

			if (safe)
				if (fs::exists(to_path))
					return 2;

			auto options = safe ?
				fs::copy_options::skip_existing : fs::copy_options::overwrite_existing;
			try
			{
				fs::copy_file(from_path, to_path, options);
			}
			catch (fs::filesystem_error& e)
			{
				last_error_msg = e.what();
				return 3;
			}
			catch (...)
			{
				last_error_msg = "";
				return 3;
			}
	#else
		int copy_file(const std::string & from_path, const std::string & to_path, bool safe)
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
	#endif
			return 0;
		}

	#ifdef FILESYSTEM_SUPPORTED
		bool remove_file(const fs::path& fpath)
		{
			return fs::remove(fpath);
		}
	#else
		bool remove_file(const std::string& fpath)
		{
			return std::remove(fpath.c_str()) == 0;
		}
	#endif
        bool remove_file(const std::string& fpath, bool b)
		{
			return std::remove(fpath.c_str()) == 0;
		}
	#ifdef FILESYSTEM_SUPPORTED
		int move_file(const fs::path& from_path, const fs::path& to_path, bool safe)
	#else
		int move_file(const std::string& from_path, const std::string& to_path, bool safe)
	#endif
		{
			auto ret = copy_file(from_path, to_path, safe);
			if (ret == 0)
				remove_file(from_path);
			return ret;
		}
	#ifdef FILESYSTEM_SUPPORTED
		int remove_last_line(const fs::path& fpath)
	#else
		int remove_last_line(const std::string& fpath)
	#endif
		{
			std::ifstream f(fpath);
			return remove_last_line_f(f);
		}

		int remove_last_line_f(std::ifstream& f)
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

	#ifdef FILESYSTEM_SUPPORTED
		std::chrono::system_clock::time_point modif_time(const fs::path& fpath)
		{
			if (!utils::file::exists(fpath))
				return std::chrono::system_clock::time_point();

	#if defined(__APPLE__) || defined(__GNUC__)
			auto tp = fs::last_write_time(fpath);
			auto sctp = ch::time_point_cast<ch::system_clock::duration>(tp - ch::file_clock::now() + ch::system_clock::now());
			auto sd = ch::duration_cast<ch::system_clock::duration>(sctp.time_since_epoch());
			return std::chrono::system_clock::time_point(sd);

	#else
			return ch::clock_cast<ch::system_clock>(fs::last_write_time(fpath));
	#endif
		}

	#else
		std::chrono::system_clock::time_point modif_time(const std::string& fpath)
		{
			throw("modif_time not supported without <filesystem>");
		}
	#endif
	}
}



