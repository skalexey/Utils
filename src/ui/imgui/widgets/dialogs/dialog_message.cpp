#include <utils/ui/imgui/widget_factory.h>
#include <utils/ui/imgui/widgets/dialogs/dialog_message.h>
#include <utils/ui/imgui/common.h>
#include <imgui.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			REGISTER_WIDGET(imgui::dialog_message);

			dialog_message::dialog_message(
				const std::string& msg
				, const base::on_answer_t& on_answer
				, const char* ok_text
				, const std::string& title
			) : imgui::dialog(), base(msg, on_answer, ok_text, title) {
				ok_button().set_on_before_show([this]() {
					ImGuiStyle& style = ImGui::GetStyle();
					float width = 0.0f;
					width += ok_button().get_size().x;
					utils::ui::imgui::AlignForWidth(width);
				});
			}

			void imgui::dialog_message::button_ok_show()
			{
				base::button_ok_show();
			}
		}
	}
}