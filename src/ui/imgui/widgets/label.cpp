#pragma once

#include <utils/ui/imgui/widget_factory.h>
#include <utils/ui/imgui/widgets/label.h>
#include <imgui.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			REGISTER_WIDGET(label);

			void imgui::label::on_show()
			{
				ImGui::Text("%s", get_text().c_str());
			}
		}
	}
}