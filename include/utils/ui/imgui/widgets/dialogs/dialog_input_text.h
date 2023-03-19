#pragma once

#include <memory>
#include <string>
#include <utils/ui/imgui/widgets/label.h>
#include <utils/ui/imgui/widgets/text_input.h>
#include <utils/ui/widgets/dialogs/dialog_input_text.h>
#include <utils/ui/imgui/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			// Order of inheritance is important here as imgui::dialog sets the widget factory first
			class dialog_input_text : public imgui::dialog, public ui::dialog_input_text
			{
				using base = ui::dialog_input_text;

			public:
				dialog_input_text() : imgui::dialog(), base() {}
				dialog_input_text(
					const std::string& msg
					, const on_result_t& on_result
					, const std::string& default_input_text = {}
					, const std::string& title = {}
					, const char* ok_btn_text = nullptr
					, const char* cancel_btn_text = nullptr
				) : imgui::dialog(), base(msg, on_result, default_input_text, title, ok_btn_text, cancel_btn_text) {}

				void on_show() override {
					imgui::dialog::on_show();
				}
			};
		}
	}
}