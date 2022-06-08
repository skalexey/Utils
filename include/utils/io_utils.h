//
//  io_utils.h
//
//  Created by Alex Skorokhodov on 2022/04/21.
//

#pragma once

#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <functional>
#include <unordered_map>
#include <string>
#include "string_utils.h"

namespace utils
{
	template <typename T>
	void print_list(T& l)
	{
		std::cout << "[";
		for (auto it = l.begin();; ++it)
		{
			std::cout << *it;
			if (it == std::prev(l.end()))
			{
				std::cout << "]";
				break;
			}
			else
			{
				std::cout << ", ";
			}
		}
	}

	namespace input
	{
		struct wr_file_info
		{
			wr_file_info() = default;
			std::ofstream fo;
			std::ifstream fi;
			std::size_t fsize = 0;
		};

		extern std::unordered_map<std::string, wr_file_info> file_info;

		// utils::getline
		extern std::string last_getline_value;
		extern std::string last_command;
		extern bool last_getline_valid;
		extern std::string last_input_fname;

		void getline(std::istream& is, std::string& s);

		// input_line, input_t, close_input
		template <typename T>
		bool input_t_impl(
			T& t
			, std::istream& def_i
			, const std::function<bool(std::istream&, T&)>& reader
			, const std::string& fname = ""
		)
		{
			if (!fname.empty())
			{
				auto it = file_info.find(fname);
				if (it != file_info.end())
				{
					auto& info = it->second;
					if (info.fi.tellg() < info.fsize)
					{
						//std::cout << "fi.tellg(): " << fi.tellg() << ", fsize: " << fsize << "\n";
						auto ret = reader(info.fi, t);
						std::cout << (ret ? "[from file]: '" : "[from file with errors]: '") << t << "'\n";
						return ret;
					}
					else
					{
						// Read from the default input
						auto ret = reader(def_i, t);
						if (last_getline_valid)
						{
							info.fo << t << "\n";
							info.fo.flush();
						}
						return ret;
					}
				}
				else
				{
					wr_file_info& info = file_info[fname];
					info.fo.open(fname, std::ios::app);
					info.fo.seekp(0, std::ios::end);
					info.fi.open(fname);
					info.fsize = info.fo.tellp();
					last_input_fname = fname;
					return input_t_impl(t, def_i, reader, fname);
				}
			}
			else
			{
				return reader(def_i, t);
			}
		}

		bool input_line(std::string& s, std::istream& def_i, const std::string& fname = "");

		template <typename T>
		bool input_t(T& t, std::istream& def_i, const std::string& fname = "")
		{
			return input_t_impl<T>(t, def_i, [&](std::istream& is, T& to) {
				std::string s;
				utils::input::getline(is, s);
				std::stringstream ss;
				ss << s;
				ss >> to;
				if (!ss.eof())
					return false;
				return true;
			}, fname);
		}

		void close_input(const std::string& fname = "");

		// register_command
		struct cmd_info
		{
			std::string alias;
			using f_t = std::function<void()>;
			f_t f;

			inline void call() {
				if (f)
					f();
			}
		};
		extern std::unordered_map<std::string, cmd_info> registered_commands;

		void register_command(const std::string& cmd, const cmd_info::f_t& f = nullptr);

		void reset_last_input();
	}
}

