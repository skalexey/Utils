#pragma once

#include <imgui.h>
#include <utils/ui/widget.h>
#include <utils/ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			vec2i imgui::widget::get_screen_size() const
			{
				auto& s = ImGui::GetIO().DisplaySize;
				return vec2i(s.x, s.y);
			}
		}
	}
}