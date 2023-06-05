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
			class label : public imgui::widget, public ui::label
			{
			public:
				using base = ui::label;

				label(ui::node* parent = nullptr)
					: ui::node(parent)
					, base(parent)
					, imgui::widget(parent)
				{}

				label(ui::node* parent, const std::string& label)
					: ui::node(parent)
					, base(parent, label)
					, imgui::widget(parent)
				{}

				void on_show() override;

			protected:
				WIDGET_REGISTRATOR(imgui::widget_factory, label);
			};
			
			using label_ptr = std::shared_ptr<label>;
		}
	}
}