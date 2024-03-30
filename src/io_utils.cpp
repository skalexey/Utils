//
//  io_utils.cpp
//
//  Created by Alex Skorokhodov on 2022/06/05.
//
#include <unordered_map>
#include <thread>
#include <utils/string_utils.h>
#include <utils/io_utils.h>

namespace utils
{
	namespace input
	{
		std::unordered_map<std::string, wr_file_info> file_info;

		// utils::getline
		struct io_context_data_t
		{
			std::string last_getline_value;
			bool last_getline_valid = true;
			std::string last_command;
			std::string last_input_fpath;

			void reset_last_input()
			{
				last_getline_value.clear();
				last_getline_valid = true;
				last_command.clear();
			}
		};

		class io_context_t
		{
			// Allow a new thread entry to be created even in the constant data getter
			std::unordered_map<std::thread::id, io_context_data_t> m_data;

		public:
			io_context_data_t& data() {
				return m_data[std::this_thread::get_id()];
			}
		};
		
		io_context_t io_context;

		std::string last_getline_value()
		{
			return io_context.data().last_getline_value;
		}

		std::string last_command()
		{
			return io_context.data().last_command;
		}

		bool last_getline_valid()
		{
			return io_context.data().last_getline_valid;
		}

		std::string& last_input_fpath()
		{
			return io_context.data().last_input_fpath;
		}

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
				auto& last_getline_value = io_context.data().last_getline_value;
				last_getline_value = tmp;
				auto first_word = last_getline_value.substr(0, last_getline_value.find_first_of(" "));

				// Set valid flag

				auto it = registered_commands.find(utils::str_tolower(first_word));
				auto& io_context_data = io_context.data();
				auto& last_getline_valid = io_context_data.last_getline_valid;
				auto& last_command = io_context_data.last_command;
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
			auto& last_input_fpath = io_context.data().last_input_fpath;
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
			auto& last_input_fpath = io_context.data().last_input_fpath;
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
			io_context.data().reset_last_input();
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

