#pragma once

#include <utils/string_utils.h>
#include <utils/ui/imgui/window.h>
#include <imgui.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			void imgui::window::on_show()
			{
				if (!is_open())
					return;
				// Set window position
				auto& pos = get_position();
				ImGui::SetNextWindowPos(
					ImVec2(pos.x, pos.y)
					, ImGuiCond_FirstUseEver
				);
				auto& sz = get_size();
				ImGui::SetNextWindowSize(
					ImVec2(sz.x, sz.y)
					, ImGuiCond_FirstUseEver
				);

				// Create the window
				bool p_open = true;
				ImGuiWindowFlags window_flags = 0;
				// TODO: check if we should use a static buffer
				if (!ImGui::Begin(utils::format_str("Window_%s", get_title()).c_str(), &p_open, window_flags))
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