#pragma once

#include <memory>
#include <utils/ui/widgets/label.h>
#include <utils/ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class label : public imgui::widget, public utils::ui::label
			{
				using base = utils::ui::label;

			public:
				label() = default;
				label(const std::string& label) : base(label) {}

				void on_show() override;
			};
			using label_ptr = std::unique_ptr<label>;
		}
	}
}