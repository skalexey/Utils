#include <utils/string_utils.h>
#include <utils/ui/imgui/widgets/dialog.h>
#include <utils/ui/imgui/widget_factory.h>
#include <imgui.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			imgui::dialog::dialog()
			{
				set_factory(widget_factory::instance());
				set_title("ImGui Dialog");
			}

			void imgui::dialog::on_show() {
				if (!is_open())
					return;
				// Set window position
				auto& pos = get_position();
				ImGui::SetNextWindowPos(
					ImVec2(pos.x, pos.y)
					, ImGuiCond_Appearing
				);
				auto& sz = get_size();
				ImGui::SetNextWindowSize(
					ImVec2(sz.x, sz.y)
					, ImGuiCond_Appearing
				);

				// Create the window
				bool p_open = true;
				ImGuiWindowFlags window_flags = 0;
				// TODO: check if we should use a static buffer
				if (!ImGui::Begin(utils::format_str("%s##", get_title().c_str()).c_str(), &p_open, window_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return;
				}

				base::on_show();

				// Close button
				if (!p_open)
					close();

				// End the window scope
				ImGui::End();
				return;
			}
		}
	}
}