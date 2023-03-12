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

			void imgui::dialog::on_show()
			{
				if (!is_open())
					return;

				auto font = ImGui::GetFont();
				font->Scale = 1.5f;
				// Set window position
				auto& pos = get_position();
				ImGui::SetNextWindowPos(
					ImVec2(float(pos.x), float(pos.y))
					, pos == m_last_position ? ImGuiCond_Appearing : ImGuiCond_Always
				);
				m_last_position = pos;
				auto& sz = get_size();
				ImGui::SetNextWindowSize(
					ImVec2(float(sz.x), float(sz.y))
					, sz == m_last_size ? ImGuiCond_Appearing : ImGuiCond_Always
				);
				m_last_size = sz;
				// Create the window
				bool p_open = true;
				auto p_open_ptr = is_close_button_enabled() ? &p_open : nullptr;
				ImGuiWindowFlags window_flags = 0;
				// TODO: check if we should use a static buffer
				if (!ImGui::Begin(utils::format_str("%s##", get_title().c_str()).c_str(), p_open_ptr, window_flags))
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