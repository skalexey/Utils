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
			label(node* parent = nullptr) : text(parent) {}
			
			label(node* parent, const std::string& label)
				: text(parent, label)
			{}

		};
		using label_ptr = std::shared_ptr<label>;
	}
}