#pragma once

#include <string>
#include <utils/ui/imgui/fwd.h>
#include <utils/ui/widgets/text_input.h>
#include <utils/ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class text_input : public imgui::widget, public ui::text_input
			{
			public:
				using base = ui::text_input;

				text_input(ui::node* parent = nullptr
					, const std::string& label = {}
					, const std::string& default_value = {}
					, const on_update_t& on_update = nullptr
				)
					: ui::node(parent)
					, base(parent, label, default_value, on_update)
					, imgui::widget(parent)
				{}

			protected:
				bool show_input() override;
				void show_text() override;

			private:
				WIDGET_REGISTRATOR(imgui::widget_factory, imgui::text_input);
			};
		}
	}
}