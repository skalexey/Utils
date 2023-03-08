#pragma once

#include <utils/string_utils.h>
#include <utils/ui/window.h>
#include "imgui.h"

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class window : public virtual ui::window
			{
				using base = ui::window;
			public:
				window() = default;
				window(const math::vector2& position, const math::vector2& size)
					: base(position, size)
				{}
				void on_show() override {
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
					if (!ImGui::Begin(utils::format_str("Window_%s", title()).c_str(), &p_open, window_flags))
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
			};

			using window_ptr = std::unique_ptr<window>;
		}
	}
}