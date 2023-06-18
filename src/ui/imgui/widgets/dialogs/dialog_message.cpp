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

			int imgui::dialog_message::post_construct()
			{
				auto retcode = base::post_construct();
				if (retcode != 0)
					return retcode;
				ok_button().set_on_before_show([this]() {
					ImGuiStyle& style = ImGui::GetStyle();
					float width = 0.0f;
					width += ok_button().get_size().x;
					utils::ui::imgui::AlignForWidth(width);
				});
				return 0;
			}

			void imgui::dialog_message::button_ok_show()
			{
				base::button_ok_show();
			}
		}
	}
}