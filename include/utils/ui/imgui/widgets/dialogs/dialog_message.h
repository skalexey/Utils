#pragma once

#include <string>
#include <utils/ui/widgets/dialogs/dialog_message.h>
#include <utils/ui/imgui/widgets/dialog.h>
#include <utils/ui/imgui/common.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			// Order of inheritance is important here as imgui::dialog sets the widget factory first
			class dialog_message : public imgui::dialog, public ui::dialog_message
			{
				using base = utils::ui::dialog_message;
			public:

				dialog_message(
					const std::string& msg
					, const base::on_answer_t& on_answer
					, const char* ok_text = nullptr
					, const std::string& title = {}
				) : imgui::dialog(), base(msg, on_answer, ok_text, title) {
					ok_button().set_on_before_show([this]() {
						ImGuiStyle& style = ImGui::GetStyle();
						float width = 0.0f;
						width += ok_button().get_size().x;
						utils::ui::imgui::AlignForWidth(width);
					});
				}

			protected:
				void on_show() override {
					imgui::dialog::on_show();
				}

				void button_ok_show() override;
			};
		}
	}
}