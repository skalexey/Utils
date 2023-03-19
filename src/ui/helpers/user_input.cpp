#include <utils/ui/app.h>
#include <utils/ui/imgui/widgets/dialogs/dialog_yes_no.h>
#include <utils/ui/imgui/widgets/dialogs/dialog_message.h>
#include <utils/ui/imgui/widgets/dialogs/dialog_input_text.h>
#include <utils/ui/helpers/user_input.h>
#include <utils/Log.h>
#include <utils/common.h>
#include <utils/string_utils.h>

LOG_PREFIX("[user_input]: ");
LOG_POSTFIX("\n");

namespace utils
{
	namespace ui
	{
			void user_input::ask_user(
				const std::string& question
				, const utils::void_bool_cb& on_answer
				, const char* yes_btn_text
				, const char* no_btn_text
			)
			{
				LOG("ask_user: '" << question << "' ['" << utils::to_str(yes_btn_text) << "'; '" << utils::to_str(no_btn_text) << "']\n");
				auto d = std::make_shared<imgui::dialog_yes_no>(question, on_answer, yes_btn_text, no_btn_text, "Question");
				m_app->add_on_update([=](float dt) {
					return d->show();
				});
			}

			void user_input::show_message(
				const std::string& message
				, const utils::void_cb& on_close
				, const char* ok_btn_text
			)
			{
				LOG("show_message: '" << message << "' ['" << utils::to_str(ok_btn_text) << "']\n");
				auto d = std::make_shared<imgui::dialog_message>(message, [=](bool) {
					if (on_close)
						on_close();
				}, ok_btn_text);
				m_app->add_on_update([=](float dt) {
					return d->show();
				});
			}

			void user_input::ask_line(
				const std::string& msg
				, const utils::void_string_bool_cb& on_answer
				, const std::string& default_value
				, const char* ok_btn_text
				, const char* cancel_btn_text
			)
			{
				LOG("ask_line: '" << msg << "' ['" << utils::to_str(ok_btn_text) << "'; '" << utils::to_str(cancel_btn_text) << "']\n");
				auto d = std::make_shared<utils::ui::imgui::dialog_input_text>(
					msg
					, [=](const std::string& path, bool cancelled) {
						on_answer(path, cancelled);
					}
					, default_value
					, ""
					, ok_btn_text
					, cancel_btn_text
				);
				m_app->add_on_update([=](float dt) {
					return d->show();
				});
			}

	}
}
