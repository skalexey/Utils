#pragma once

#include <utils/ui/imgui/widget_factory.h>
#include <utils/ui/imgui/widgets/text.h>
#include <imgui.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			REGISTER_WIDGET(text);

			void imgui::text::on_show()
			{
				ImGui::TextWrapped("%s", get_text().c_str());
			}
		}
	}
}