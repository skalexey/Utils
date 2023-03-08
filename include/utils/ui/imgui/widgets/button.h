#pragma once

#include <memory>
#include <utils/ui/widgets/button.h>
#include "imgui.h"

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class button : public utils::ui::button
			{
			public:
				using base = utils::ui::button;
				button() = default;
				button(const std::string& label, const on_click_t& on_click = nullptr) : base(label, on_click) {}

				void on_show() override {
					if (ImGui::Button(get_text().c_str()))
						on_click(true);
				}
			};
			using button_ptr = std::shared_ptr<button>;
		}
	}
}