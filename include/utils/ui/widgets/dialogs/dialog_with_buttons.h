#pragma once

#include <vector>
#include <string>
#include <optional>
#include <utils/ui/widgets/dialog.h>
#include <utils/ui/widgets/button.h>
#include <utils/ui/widgets/label.h>
#include <utils/ui/widget_factory.h>

namespace utils
{
	namespace ui
	{
		class dialog_with_buttons : public virtual dialog
		{
		public:
			using actions_t = std::vector<std::pair<std::string, button::on_click_t>>;

			dialog_with_buttons()
			{
				set_title("Dialog With Buttons");
				// Factory here is supposed to be already set in the implementation class
				set_message_label(get_factory().create_label());
				set_on_show([this]() {
					m_lbl_message->show();
					for (auto& btn : m_buttons)
						btn->show();
				});
			}

			dialog_with_buttons(
				const std::string& msg
				, const actions_t& actions
				, const std::optional<std::string>& title = {}
			)
				: dialog_with_buttons()
			{
				if (title.has_value())
					set_title(title.value());
				set_message(msg);
				setup_buttons(actions);
			}

			void setup_buttons(const actions_t& actions) {
				for (auto& action : actions)
					add_button(action.first, action.second);
			}

			label& message_label() { return *m_lbl_message; }

			void add_button(const std::string& text, const button::on_click_t& on_click = nullptr) {
				auto btn = get_factory().create_button();
				btn->set_text(text);
				btn->set_on_click(on_click);
				add_button(btn);
			}
			void set_message(const std::string& message)
			{
				if (!m_lbl_message)
					m_lbl_message = get_factory().create_label();
				m_lbl_message->set_text(message);
			}

		protected:
			void add_button(const ui::button_ptr& btn) {
				m_buttons.push_back(btn);
			}
			void set_message_label(const label_ptr& lbl) {
				m_lbl_message = lbl;
			}

		protected:
			std::vector<button_ptr> m_buttons;

		private:
			label_ptr m_lbl_message;
		};
		using dialog_with_buttons_ptr = std::unique_ptr<dialog_with_buttons>;
	}
}