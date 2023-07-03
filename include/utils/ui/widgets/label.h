#pragma once

#include <memory>
#include <string>
#include <utils/ui/widget.h>
#include <utils/ui/widgets/text.h>

namespace utils
{
	namespace ui
	{
		// Logically, Label and Text are the same. They only differ in UI implementation.
		class label : public text {};
		using label_ptr = std::shared_ptr<label>;
	}
}