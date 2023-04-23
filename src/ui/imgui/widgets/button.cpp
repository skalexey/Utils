#include <cmath>
#include <imgui.h>
#include <utils/ui/imgui/widgets/button.h>
#include <utils/ui/imgui/widget_factory.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			REGISTER_WIDGET(imgui::button);

			void button::on_show()
			{
				#ifdef ANDROID
					auto ts = text_size();
					vec2i new_size = {std::max(100, ts.x + 20), 100};
					set_size(new_size);
					const ImVec2 sz = {float(new_size.x), float(new_size.y)};
				#else
					const ImVec2 sz = {0, 0};
				#endif
				if (ImGui::Button(get_text().c_str(), sz))
					on_click(true);
				imgui::widget::on_show();
			}

			const vec2i& button::text_size()
			{
				auto s = ImGui::CalcTextSize(get_text().c_str());
				return m_calculated_size = { (int)s.x, (int)s.y };
			}
		}
	}
}