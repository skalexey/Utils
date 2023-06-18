#include <utils/ui/app.h>
#include <utils/ui/widgets/dialogs/dialog_yes_no.h>
#include <utils/ui/widgets/dialogs/dialog_message.h>
#include <utils/ui/widgets/dialogs/dialog_input_text.h>
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
				auto d = m_app->get_factory().create<dialog_yes_no>(m_app);
				d->set_on_answer(on_answer);
				d->set_message(question);
				d->set_yes_text(yes_btn_text ? yes_btn_text : "Yes");
				d->set_no_text(no_btn_text ? no_btn_text : "No");
				d->set_title("Question");
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
				auto d = m_app->get_factory().create<dialog_message>(m_app);
				d->set_message(message);
				d->set_on_answer([=](bool) {
					if (on_close)
						on_close();
				});
				d->set_ok_text(ok_btn_text ? ok_btn_text : "Ok");
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
				auto d = m_app->get_factory().create<dialog_input_text>(m_app);
				// TODO: move this all into an Init() method.
				d->message_label().set_text(msg);
				d->set_on_result([=](const std::string& path, bool cancelled) {
					on_answer(path, cancelled);
				});
				d->text_input().set_value(default_value);
				d->text_input().set_label("");
				d->ok_button().set_text(ok_btn_text ? ok_btn_text : "Submit");
				d->cancel_button().set_text(cancel_btn_text ? cancel_btn_text : "Cancel");
				m_app->add_on_update([=](float dt) {
					return d->show();
				});
			}

	}
}
