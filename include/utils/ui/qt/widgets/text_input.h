#pragma once

#include <string>
#include <utils/ui/qt/fwd.h>
#include <utils/ui/widgets/text_input.h>
#include <utils/ui/qt/widget.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class text_input : public qt::widget, public utils::ui::text_input
			{
			public:
				using base = utils::ui::text_input;

				text_input(ui::node* parent = nullptr
					, const std::string& label = {}
					, const std::string& default_value = {}
					, const on_update_t& on_update = nullptr
				)
					: ui::node(parent)
					, base(parent, label, default_value, on_update)
					, qt::widget(parent)
				{}

			protected:
				bool show_input() override;
				void show_text() override;

			private:
				WIDGET_REGISTRATOR(qt::widget_factory, text_input);
			};
		}
	}
}