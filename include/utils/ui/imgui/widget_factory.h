#pragma once

#include <utils/ui/widget_factory.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class widget_factory : public utils::ui::widget_factory
			{
			public:
				static widget_factory& instance();
				// TODO: use variadic arguments list
				virtual utils::ui::text_ptr create_text() const;
				virtual utils::ui::label_ptr create_label() const;
				virtual utils::ui::button_ptr create_button() const;
				virtual utils::ui::text_input_ptr create_text_input() const;
			};
		}
	}
}