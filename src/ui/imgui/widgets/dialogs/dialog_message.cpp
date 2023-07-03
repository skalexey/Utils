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

			imgui::dialog_message::dialog_message()
			{
				do_on_post_construct([self = this] {
					self->ok_button().set_on_before_update([self](float dt) {
						ImGuiStyle& style = ImGui::GetStyle();
						float width = 0.0f;
						width += self->ok_button().get_size().x;
						utils::ui::imgui::AlignForWidth(width);
						return true;
					});
					return 0;
				});
			}
		}
	}
}