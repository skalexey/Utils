#pragma once

#include <memory>
#include <utils/ui/widgets/label.h>
#include "imgui.h"

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class label : public utils::ui::label
			{
				using base = utils::ui::label;

			public:
				label() = default;
				label(const std::string& label) : base(label) {}

				void on_show() override {
					ImGui::Text("%s", get_text().c_str());
				}
			};
			using label_ptr = std::unique_ptr<label>;
		}
	}
}