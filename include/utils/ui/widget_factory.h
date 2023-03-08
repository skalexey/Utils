#pragma once

#include <utils/ui/widgets/label.h>
#include <utils/ui/widgets/button.h>
#include <utils/ui/widgets/text_input.h>

namespace utils
{
	namespace ui
	{
		class widget_factory
		{
		public:
			// TODO: use variadic arguments list
			virtual label_ptr create_label() const { return nullptr; };
			virtual button_ptr create_button() const { return nullptr; };
			virtual text_input_ptr create_text_input() const { return nullptr; };
		};
	}
}