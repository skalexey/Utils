#pragma once

#include <vector>
#include <string>
#include <utils/ui/widgets/dialog.h>
#include <utils/ui/widgets/button.h>
#include <utils/ui/widgets/label.h>
#include <utils/ui/widgets/text_input.h>
#include <utils/ui/widget_factory.h>

namespace utils
{
	namespace ui
	{
		class dialog_input_text : public virtual dialog
		{
			using base = dialog;

			public:
				using on_result_t = std::function<void(const std::string& result, bool cancelled)>;
				dialog_input_text() {
					// Factory here is supposed to be already created in the implementation class
					m_message_label = get_factory().create_label();
					m_text_input = get_factory().create_text_input();
					m_ok_button = get_factory().create_button();
					m_cancel_button = get_factory().create_button();
					set_on_show([this]() {
						on_show();
					});
				}
				dialog_input_text(
					const std::string& msg
					, const on_result_t& on_result
					, const std::string& default_input_text
					, const std::string& title
				)
					: dialog_input_text()
				{
					m_on_result = on_result;
					m_message_label->set_text(msg);
					m_text_input->set_value(default_input_text);
					// TODO: remove this label
					m_text_input->set_label("Answer");
					m_ok_button->set_text("Ok");
					m_ok_button->set_on_click([this](bool up) {
						if (m_on_result)
							m_on_result(m_text_input->get_value(), false);
						close();
					});
					m_cancel_button->set_text("Cancel");
					m_cancel_button->set_on_click([this](bool up) {
						if (m_on_result)
							m_on_result(m_text_input->get_value(), true);
						close();
					});
				}

				label& message_label() { return *m_message_label; }
				text_input& text_input() { return *m_text_input; }

			protected:
				void on_show() override {
					m_message_label->show();
					m_ok_button->show();
					m_cancel_button->show();
					m_text_input->show();
				};

			protected:
				label_ptr m_message_label;
				button_ptr m_ok_button;
				button_ptr m_cancel_button;
				text_input_ptr m_text_input;
				on_result_t m_on_result;
		};
		using dialog_input_text_ptr = std::unique_ptr<dialog_input_text>;
	}
}