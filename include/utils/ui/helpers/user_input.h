#pragma once

#include <functional>
#include <string>

class app;

namespace utils
{
	namespace ui
	{
		class user_input
		{
		public:
			user_input(app* app) : m_app(app) {}

			void ask_user(
				const std::string& question
				, const std::function<void(bool)>& on_answer
				, const char* yes_btn_text = nullptr
				, const char* no_btn_text = nullptr
			);

			void show_message(
				const std::string& message
				, const std::function<void()>& on_close = nullptr
				, const char* ok_btn_text = nullptr
			);

		private:
			app* m_app;
		};
	}
}
