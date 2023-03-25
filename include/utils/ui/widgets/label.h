#pragma once

#include <memory>
#include <string>
#include <utils/ui/widget.h>
#include <utils/ui/widgets/text.h>

namespace utils
{
	namespace ui
	{
		class label : public text
		{
		public:
			label() = default;
			label(const std::string& label)
				: text()
			{}

		};
		using label_ptr = std::shared_ptr<label>;
	}
}