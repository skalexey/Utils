#include <utils/ui/imgui/widget_factory.h>
#include <utils/ui/imgui/widgets/dialogs/dialog_yes_no.h>
#include <utils/ui/imgui/common.h>
#include <imgui.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			REGISTER_WIDGET(imgui::dialog_yes_no);

			dialog_yes_no::dialog_yes_no(
				const std::string& msg
				, const on_answer_t& on_answer
				, const char* yes_text
				, const char* no_text
				, const std::optional<std::string>& title
			) : imgui::dialog(), base(msg, on_answer, yes_text, no_text, title) {
				yes_button().set_on_before_show([this]() {
					ImGuiStyle& style = ImGui::GetStyle();
					float width = 0.0f;
					width += yes_button().get_size().x;
					width += style.ItemSpacing.x;
					width += 60.0f;
					width += no_button().get_size().x;
					utils::ui::imgui::AlignForWidth(width);
				});
				yes_button().set_on_show([this]() {

					ImGui::SameLine();
					ImGui::Dummy({ 60, 0 });
					ImGui::SameLine();
				});
			}

			void imgui::dialog_yes_no::button_yes_show()
			{
				base::button_yes_show();
				ImGui::SameLine();
			}
		}
	}
}