#pragma once

#include <memory>
#include <string>
#include <utils/ui/widgets/value_input.h>

namespace utils
{
	namespace ui
	{
		using text_input = value_input<std::string>;
		using text_input_ptr = std::shared_ptr<text_input>;
	}
}