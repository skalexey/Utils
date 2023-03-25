#pragma once

#include <cmath>
#include <memory>
#include <imgui.h>
#include <utils/ui/widgets/button.h>
#include <utils/ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class button : public imgui::widget, public utils::ui::button
			{
			public:
				using base = utils::ui::button;
				button() = default;
				button(const std::string& label, const on_click_t& on_click = nullptr) : base(label, on_click) {}

				void on_show() override {
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

				const vec2i& text_size() {
					auto s = ImGui::CalcTextSize(get_text().c_str());
					return m_calculated_size = { (int)s.x, (int)s.y };
				}

				const vec2i& get_size() {
					return m_calculated_size;
				}

				private:
					vec2i m_calculated_size;
			};
			using button_ptr = std::shared_ptr<button>;
		}
	}
}