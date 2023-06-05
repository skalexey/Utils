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

			void imgui::dialog_message::init()
			{
				ok_button().set_on_before_show([this]() {
					ImGuiStyle& style = ImGui::GetStyle();
					float width = 0.0f;
					width += ok_button().get_size().x;
					utils::ui::imgui::AlignForWidth(width);
				});
			}

			imgui::dialog_message::dialog_message(ui::node* parent)
				: ui::node(parent)
				, base(parent)
				, imgui::dialog(parent)
			{
				init();
			}

			imgui::dialog_message::dialog_message::dialog_message(
				ui::node* parent
				, const std::string& msg
				, const base::on_answer_t& on_answer
				, const char* ok_text
				, const std::string& title
			)
				: ui::node(parent)
				, base(parent, msg, on_answer, ok_text, title)
				, imgui::dialog(parent)
			{
				init();
			}

			void imgui::dialog_message::button_ok_show()
			{
				base::button_ok_show();
			}
		}
	}
}