#pragma once

#include <string>
#include <utils/ui/widgets/dialogs/dialog_with_buttons.h>
#include <utils/common.h>

namespace utils
{
	namespace ui
	{
		class dialog_message : public dialog_with_buttons
		{
			const std::string ok_text_default = "ok";

			using base = utils::ui::dialog_with_buttons;

			public:
				using on_answer_t = utils::void_bool_cb;

				dialog_message(
					const std::string& msg
					, const on_answer_t& on_answer
					, const char* ok_text
					, const std::string& title
				)
				{
					add_button(get_factory().create_button());
					set_message(msg);
					set_on_answer(on_answer);
					ok_button().set_text(ok_text ? ok_text : ok_text_default.c_str());
					ok_button().set_on_click([this](bool up) {
						this->on_answer(true);
					});
				}

				void set_on_answer(const on_answer_t& on_answer) {
					m_on_answer = on_answer;
				}
				void set_ok_text(const std::string& text) {
					ok_button().set_text(text);
				}

				button& ok_button() { return *m_buttons[0]; }

			protected:
				virtual void button_ok_show() {
					ok_button().show();
				}

				virtual void message_label_show() {
					message_label().show();
				}

				void on_show() override {
					message_label_show();
					button_ok_show();
				}

				void on_answer(bool answer) {
					m_on_answer(answer);
					close();
				}

			private:
				on_answer_t m_on_answer;
		};
	}
}