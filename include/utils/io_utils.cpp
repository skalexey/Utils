//
//  io_utils.cpp
//
//  Created by Alex Skorokhodov on 2022/06/05.
//

#include "io_utils.h"

namespace utils
{
	namespace input
	{
		std::unordered_map<std::string, wr_file_info> file_info;

		// utils::getline
		std::string last_getline_value;
		bool last_getline_valid = false;
		std::string last_command;

		void getline(std::istream& is, std::string& s)
		{
			std::getline(is, s);

			// Set last getline value
			last_getline_value = s;
			auto first_word = last_getline_value.substr(0, last_getline_value.find_first_of(" "));

			// Set valid flag

			auto it = registered_commands.find(utils::str_tolower(first_word));
			if (!(last_getline_valid = it == registered_commands.end()))
			{
				it->second.call(); // Call a command
				last_command = it->first;
			}
		}

		// input_line, close_input
		std::string last_input_fname;

		bool input_line(std::string& s, std::istream& def_i, const std::string& fname)
		{
			return input_t_impl<std::string>(s, def_i, [&](std::istream& is, std::string& to) {
				utils::input::getline(is, to);
				return true;
			}, fname);
		}

		void close_input(const std::string& fname)
		{
			auto _fname_ = fname.empty() ? last_input_fname : fname;
			if (!_fname_.empty())
			{
				auto it = file_info.find(_fname_);
				if (it != file_info.end())
				{
					auto& info = it->second;
					info.fi.close();
					info.fo.close();
					last_input_fname = "";
					file_info.erase(it);
				}
			}
		}

		// register_command
		std::unordered_map<std::string, cmd_info> registered_commands;

		void register_command(const std::string& cmd, const cmd_info::f_t& f)
		{
			registered_commands[cmd] = {cmd, f};
		}

		void reset_last_input()
		{
			last_command = "";
			last_getline_valid = false;
			last_getline_value = "";
		}
	}
}

