#include <utils/ui/app.h>
#include <utils/ui/imgui/widgets/dialogs/dialog_yes_no.h>
#include <utils/ui/imgui/widgets/dialogs/dialog_message.h>
#include <utils/ui/helpers/user_input.h>

namespace utils
{
	namespace ui
	{
			void user_input::ask_user(
				const std::string& question
				, const std::function<void(bool)>& on_answer
				, const char* yes_btn_text
				, const char* no_btn_text
			)
			{
				auto d = std::make_shared<imgui::dialog_yes_no>(question, on_answer, yes_btn_text, no_btn_text, "Question");
				m_app->add_on_update([=](float dt) {
					return d->show();
				});
			}

			void user_input::show_message(
				const std::string& message
				, const std::function<void()>& on_close
				, const char* ok_btn_text
			)
			{
				auto d = std::make_shared<imgui::dialog_message>(message, [=](bool) {
					if (on_close)
						on_close();
				}, ok_btn_text);
				m_app->add_on_update([=](float dt) {
					return d->show();
				});
			}

	}
}
