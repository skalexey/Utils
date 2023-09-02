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
			bool imgui::window::on_update(float dt)
			{
				if (!is_open())
					return false;
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
				// TODO: optimize
				if (!ImGui::Begin(utils::format_str("Window_%s", get_title().c_str()).c_str(), &p_open, window_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return true;
				}
					
				if (!base::on_update(dt))
					return false;

				if (!on_imgui_window_update(dt))
					return false;

				// Close button
				if (!p_open)
					close();
					
				// End the window scope
				ImGui::End();
				return true;
			}
		}
	}
}