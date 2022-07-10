#include <fstream>
#include <string>
#ifdef __cpp_lib_filesystem
namespace fs = std::filesystem;
#else
#include <cstdio>
#endif
#include "file_utils.h"

namespace ch = std::chrono;

namespace utils
{
	int file_exists(const std::string& fpath)
	{
		return std::ifstream(fpath.c_str()).good();
	}

#ifdef __cpp_lib_filesystem
	int copy_file(const fs::path& from_path, const fs::path& to_path, bool safe)
	{
		if (!fs::exists(from_path))
			return 1;

		if (safe)
			if (fs::exists(to_path))
				return 2;

		auto options = safe ? 
			fs::copy_options::skip_existing : fs::copy_options::overwrite_existing;
		fs::copy_file(from_path, to_path, fs::copy_options::skip_existing);
#else
	int copy_file(const std::string & from_path, const std::string & to_path, bool safe)
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

#ifdef __cpp_lib_filesystem
	bool remove_file(const fs::path& fpath)
	{
		return fs::remove(fpath);
	}
#else
	bool remove_file(const std::string& fpath)
	{
		return std::remove(from_path.c_str()) == 0;
	}
#endif


#ifdef __cpp_lib_filesystem
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
#ifdef __cpp_lib_filesystem
	int file_remove_last_line(const fs::path& fpath)
#else
	int file_remove_last_line(const std::string& fpath)
#endif
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

	inline std::string file_contents_impl(const std::string& fpath)
	{
		std::ifstream f(fpath);
		return std::string((std::istreambuf_iterator<char>(f)),
			(std::istreambuf_iterator<char>()));
	}
#ifdef __cpp_lib_filesystem
	std::chrono::system_clock::time_point file_modif_time(const fs::path& fpath)
	{
		return ch::clock_cast<ch::system_clock>(fs::last_write_time(fpath));
	}

	std::string file_contents(const fs::path& fpath)
	{
		return file_contents_impl(fpath.string());
	}
#else
	std::chrono::system_clock::time_point file_modif_time(const std::string& fpath)
	{
		throw("file_modif_time not supported without <filesystem>");
	}

	std::string file_contents(const std::string& fpath)
	{
		return file_contents_impl(fpath);
	}
#endif
}



