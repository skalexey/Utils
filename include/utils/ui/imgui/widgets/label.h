#pragma once

#include <memory>
#include <utils/ui/imgui/fwd.h>
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
			public:
				using base = ui::label;

				label() = default;
				label(const std::string& label) : base(label) {}

				void on_show() override;

			protected:
				WIDGET_REGISTRATOR(imgui::widget_factory, label);
			};
			using label_ptr = std::shared_ptr<label>;
		}
	}
}