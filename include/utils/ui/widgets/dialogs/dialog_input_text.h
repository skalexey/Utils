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
					m_message_label = get_factory().create<ui::label>();
					m_text_input = get_factory().create<ui::text_input>();
					m_ok_button = get_factory().create<ui::button>();
					m_cancel_button = get_factory().create<ui::button>();
					m_ok_button->set_on_click([this](bool up) {
						if (m_on_result)
							m_on_result(m_text_input->get_value(), false);
						close();
					});
					m_cancel_button->set_on_click([this](bool up) {
						if (m_on_result)
							m_on_result(m_text_input->get_value(), true);
						close();
					});
					set_on_show([this]() {
						m_message_label->show();
						m_ok_button->show();
						m_cancel_button->show();
						m_text_input->show();
					});
				}
				dialog_input_text(
					const std::string& msg
					, const on_result_t& on_result
					, const std::string& default_input_text
					, const std::string& title
					, const char* ok_btn_text = nullptr
					, const char* cancel_btn_text = nullptr
				)
					: dialog_input_text()
				{
					set_on_result(on_result);
					m_message_label->set_text(msg);
					m_text_input->set_value(default_input_text);
					// TODO: remove this label
					m_text_input->set_label(title);
					m_ok_button->set_text(ok_btn_text ? ok_btn_text : "Ok");
					m_cancel_button->set_text(cancel_btn_text ? cancel_btn_text : "Cancel");
				}

				label& message_label() { return *m_message_label; }
				text_input& text_input() { return *m_text_input; }
				button& ok_button() { return *m_ok_button; }
				button& cancel_button() { return *m_cancel_button; }

				void set_on_result(const on_result_t& on_result) {
					m_on_result = on_result;
				}
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