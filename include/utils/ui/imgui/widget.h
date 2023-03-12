#pragma once

#include <imgui.h>
#include <utils/ui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class widget : public virtual ui::widget
			{
				public:
					vec2i get_screen_size() const override {
						auto& s = ImGui::GetIO().DisplaySize;
						return vec2i(s.x, s.y);
					}
			};

			using widget_ptr = std::shared_ptr<imgui::widget>;
		}
	}
}