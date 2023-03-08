#pragma once

#include <utils/ui/widgets/dialogs/dialog_yes_no.h>
#include <utils/ui/imgui/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class dialog_yes_no : public imgui::dialog, public ui::dialog_yes_no
			{
				using base = ui::dialog_yes_no;

			public:
				// Order of initialization is important here as imgui::dialog sets the widget factory first
				dialog_yes_no() : imgui::dialog(), base() {}
				dialog_yes_no(
					const std::string& msg
					, const on_answer_t& on_answer
					, const char* yes_text = nullptr
					, const char* no_text = nullptr
					, const std::optional<std::string>& title = {}
				) : imgui::dialog(), base(msg, on_answer, yes_text, no_text, title) {}

				void on_show() override {
					imgui::dialog::on_show();
				}

			protected:
				void button_yes_show() override;
			};
		}
	}
}