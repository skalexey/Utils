#pragma once

#include <optional>
#include <string>
#include <utils/ui/widgets/dialogs/dialog_with_buttons.h>
#include <utils/ui/widgets/button.h>
#include <utils/ui/widgets/label.h>
#include <utils/ui/widget_factory.h>
#include <utils/common.h>

namespace utils
{
	namespace ui
	{
		class dialog_yes_no : public dialog_with_buttons
		{
			const std::string yes_text_default = "Yes";
			const std::string no_text_default = "No";

			using base = utils::ui::dialog_with_buttons;

			public:
				using on_answer_t = utils::void_bool_cb;

				dialog_yes_no()
				{
					set_title("Dialog Yes No");
					add_button(get_factory().create<ui::button>());
					add_button(get_factory().create<ui::button>());
					yes_button().set_on_click([this](bool up) {
						if (up)
							this->on_answer(true);
					});
					no_button().set_on_click([this](bool up) {
						if (up)
							this->on_answer(false);
					});
				}

				dialog_yes_no(
					const std::string& msg
					, const on_answer_t& on_answer
					, const char* yes_text = nullptr
					, const char* no_text = nullptr
					, const std::optional<std::string>& title = {}
				)
					: dialog_yes_no()
				{
					if (title.has_value())
						set_title(title.value());
					set_message(msg);
					set_on_answer(on_answer);
					yes_button().set_text(yes_text ? yes_text : yes_text_default.c_str());
					no_button().set_text(no_text ? no_text : no_text_default.c_str());
				}

				void set_on_answer(const on_answer_t& on_answer) {
					m_on_answer = on_answer;
				}
				void set_yes_text(const std::string& text) {
					yes_button().set_text(text);
				}

				void set_no_text(const std::string& text) {
					no_button().set_text(text);
				}

				button& yes_button() { return *m_buttons[0]; }
				button& no_button() { return *m_buttons[1]; }

			protected:
				virtual void button_yes_show() {
					yes_button().show();
				}

				virtual void button_no_show() {
					no_button().show();
				}

				virtual void message_label_show() {
					message_text().show();
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