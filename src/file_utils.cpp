#include <iostream>
#include <cstdlib>
#include <type_traits>
#include <fstream>
#include <string>
#include <chrono>
#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <unistd.h>
#endif
#ifdef WIN32
#define stat _stat
#endif

#include <utils/file_utils.h>
#include <utils/log.h>
LOG_TITLE("file_utils");
#ifndef FILESYSTEM_SUPPORTED
#include <cstdio>
#endif

namespace ch = std::chrono;

#ifndef UTILS_FILE_LOG_ERROR
#define UTILS_FILE_LOG_ERROR(x)
#endif

namespace
{
	std::string last_error_msg;
	int last_error_code = 0;
}

#ifdef ANDROID
	extern std::string android_get_temp_dir();
#endif

namespace utils
{
	namespace file
	{
		enum erc : int
		{
			no_error = 0,
			cant_open = 1
		};

		const std::string& last_error()
		{
			return last_error_msg;
		}

		int last_errcode()
		{
			return last_error_code;
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

		bool exists(const std::string &fpath)
		{
			return std::ifstream(fpath.c_str()).good();
		}

		inline std::vector<std::string> lines_impl(const std::string& path)
		{
			const int reserve_size = 1024;
			using namespace std;
			vector<string> o;
			o.reserve(reserve_size);
			fstream f;
			f.open(path.c_str(), ios::in | ios::binary);
			if (f.is_open())
			{
				auto it = o.begin();
				std::string s;
				while (getline(f, s))
				{
					if (!s.empty())
						if (s.back() == '\r')
							s.resize(s.size() - 1);
					o.resize(o.size() + 1);
					o.back().swap(s);
					if (o.size() == o.capacity())
						o.reserve(o.capacity() + reserve_size);
				}
				f.close();
			}
			else
			{
				last_error_code = erc::cant_open;
				UTILS_FILE_LOG_ERROR("Failed to open file '" << path << "'");
			}
			last_error_code = erc::no_error;
			return o;
		}

		inline int create_impl(const std::string& path)
		{
			std::fstream f;
			f.open(path.c_str(), std::ios::out);
			if (f.is_open())
				return 0;
			return 1;
		}

		int copy_overwrite(const std::string& from_path, const std::string& to_path, bool safe)
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
	#ifdef FILESYSTEM_SUPPORTED
		bool is_directory(const fs::path& path)
		{
			return fs::is_directory(path);
		}
		fs::path temp_directory_path()
		{
#ifdef ANDROID
			return fs::path(android_get_temp_dir());
#else
			return fs::temp_directory_path();
#endif
		}

		fs::path app_data_directory_path(const std::string& app_name)
		{
			std::filesystem::path app_data_path;

#ifdef _WIN32
			const char* env_appdata = std::getenv("APPDATA");
			if (env_appdata != nullptr) {
				app_data_path = std::filesystem::path(env_appdata) / app_name;
			}
			else {
				const char* env_userprofile = std::getenv("USERPROFILE");
				if (env_userprofile != nullptr) {
					app_data_path = std::filesystem::path(env_userprofile) / "AppData" / "Roaming" / app_name;
				}
			}
#else
			const char* env_xdg_config_home = std::getenv("XDG_CONFIG_HOME");
			if (env_xdg_config_home != nullptr) {
				app_data_path = std::filesystem::path(env_xdg_config_home) / app_name;
			}
			else {
				const char* env_home = std::getenv("HOME");
				if (env_home != nullptr) {
					app_data_path = std::filesystem::path(env_home) / ".config" / app_name;
				}
			}
#endif

			return app_data_path;
		}

		bool is_file_path(const fs::path& path)
		{
			return path.has_filename();
		}
		
		int create(const fs::path& path)
		{
			return create_impl(path.string());
		}

		std::vector<std::string> lines(const std::filesystem::path &path)
		{
			return lines_impl(path.string());
		}

		bool exists(const std::filesystem::path& fpath)
		{
			return fs::exists(fpath);
		}

		bool dir_exists(const std::filesystem::path& path)
		{
			return fs::exists(path);
		}

		int copy(const fs::path& from_path, const fs::path& to_path, bool safe, bool overwrite)
		{
			if (overwrite)
				return copy_overwrite(from_path.string(), to_path.string(), safe);

			if (!fs::exists(from_path))
				return 1;

			if (safe)
				if (fs::exists(to_path))
					return 2;

			auto options = safe ?
				fs::copy_options::skip_existing : fs::copy_options::overwrite_existing;
			try
			{
				fs::copy(from_path, to_path, options);
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
			return 0;
	#else
		bool is_directory(const std::string& path)
		{
			struct stat s;
			if (stat(path, &s) == 0)
			{
				if (s.st_mode & S_IFDIR)
				{
					return true;
				}
				else if (s.st_mode & S_IFREG)
				{
					// it's a file
					return false;
				}
				else
				{
					// something else
					return false;
				}
			}
			else
			{
				// error
				return false;
			}
		}

		std::string temp_directory_path()
		{
			return std::tmpnam(nullptr);
		}

		std::string app_data_directory_path(const std::string& app_name)
		{
			assert(false) // Not ported yet
			return temp_directory_path()
		}
		
		int create(const std::string& path)
		{
			return create_impl(path);
		}

		std::vector<std::string> lines(const std::string& path)
		{
			return lines_impl(path);
		}

		int copy(const std::string & from_path, const std::string & to_path, bool safe, bool overwrite)
        {
			return copy_overwrite(from_path, to_path, safe);
	#endif
		}

	#ifdef FILESYSTEM_SUPPORTED
		bool remove(const fs::path& fpath)
		{
			return fs::remove(fpath);
		}
	#else
		bool remove(const std::string& fpath)
		{
			return std::remove(fpath.c_str()) == 0;
		}
	#endif
        bool remove(const std::string& fpath, bool b)
		{
			return std::remove(fpath.c_str()) == 0;
		}
	#ifdef FILESYSTEM_SUPPORTED
		int move(const fs::path& from_path, const fs::path& to_path, bool safe)
	#else
		int move(const std::string& from_path, const std::string& to_path, bool safe)
	#endif
		{
			auto ret = copy(from_path, to_path, safe);
			if (ret == 0)
				utils::file::remove(from_path);
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
	#if __cplusplus < 202002L
		#if defined(_WIN32)
			auto tp = fs::last_write_time(fpath);
			auto now = std::filesystem::file_time_type::clock::now();
			auto dur = ch::system_clock::time_point::duration(now.time_since_epoch().count() - tp.time_since_epoch().count());
			auto r = ch::system_clock::now() - dur;
			return r;
		#else
			// This code words on Windows too
			struct stat result;
			if(stat(fpath.string().c_str(), &result)==0)
				return std::chrono::system_clock::from_time_t(result.st_mtime);
			return std::chrono::system_clock::time_point();
		#endif
	#elif defined(__APPLE__) || defined(__GNUC__)
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



