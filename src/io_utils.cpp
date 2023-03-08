//
//  io_utils.cpp
//
//  Created by Alex Skorokhodov on 2022/06/05.
//

#include <utils/string_utils.h>
#include <utils/io_utils.h>

namespace utils
{
	namespace input
	{
		std::unordered_map<std::string, wr_file_info> file_info;

		// utils::getline
		std::string last_getline_value;
		bool last_getline_valid = true;
		std::string last_command;

		bool getline(std::istream& is, std::string& s)
		{
			std::string tmp;
			bool cmd_ret = false;
			do
			{
				std::getline(is, tmp);
				if (std::cin.fail())
					return false;
				// Set last getline value
				last_getline_value = tmp;
				auto first_word = last_getline_value.substr(0, last_getline_value.find_first_of(" "));

				// Set valid flag

				auto it = registered_commands.find(utils::str_tolower(first_word));
				if (!(last_getline_valid = it == registered_commands.end()))
				{
					cmd_ret = it->second.call(); // Call a command
					last_command = it->first;
					tmp.clear();
				}
			} while (cmd_ret && !last_getline_valid);

			tmp.swap(s);
			return true;
		}

		// input_line, close_input
		std::string last_input_fpath;

		bool input_line(std::string& s, std::istream& def_i, const std::string& fpath)
		{
			return input_t_impl<std::string>(s, def_i
				, [&](std::istream& is, std::string& to) {
					utils::input::getline(is, to);
					return true;
				}
			, fpath);
		}

		void close_input(const std::string& fpath)
		{
			auto _fpath_ = fpath.empty() ? last_input_fpath : fpath;
			if (!_fpath_.empty())
			{
				auto it = file_info.find(_fpath_);
				if (it != file_info.end())
				{
					auto& info = it->second;
					info.fi.close();
					info.fo.close();
					last_input_fpath = "";
					file_info.erase(it);
				}
			}
		}

		std::ifstream* get_file(const std::string& fpath)
		{
			auto _fpath_ = fpath.empty() ? last_input_fpath : fpath;
			if (!_fpath_.empty())
			{
				auto it = file_info.find(_fpath_);
				if (it != file_info.end())
					return &it->second.fi;
			}
			return nullptr;
		}

		// register_command
		std::unordered_map<std::string, cmd_info> registered_commands;

		void register_command(const std::string& cmd, const cmd_info::f_t& f)
		{
			registered_commands[cmd] = {cmd, f};
		}

		void unregister_command(const std::string& cmd)
		{
			registered_commands.erase(cmd);
		}

		void reset_last_input()
		{
			last_command = "";
			last_getline_valid = false;
			last_getline_value = "";
		}
        
		bool ask_user(const std::string& question)
        {
			auto repeat_please = [] () {
				std::cout << "Answer y or n (or yes or no)\n";
			};

			std::string answer;
			std::cout << question << " (y/n)\n";

			while (!std::cin.fail())
			{
				std::getline(std::cin, answer);
				auto a = utils::str_tolower(answer);
				if (a == "y" || a == "yes")
					return true;
				else if (a == "n" || a == "no")
					return false;
				else
					repeat_please();
			}
			throw std::string("Input stream failure");
        }

		bool ask_line(std::string& to, const std::string& msg, const std::string& repeat_msg)
		{
			if (!msg.empty())
				std::cout << msg;
			bool tried = false;
			do
			{
				if (tried)
					std::cout << repeat_msg;
				if (!utils::input::getline(std::cin, to))
					return false;
				tried = true;
			} while (msg.empty());
			return true;
		}
	}
}

