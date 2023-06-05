#pragma once

#include <memory>
#include <string>
#include <utils/ui/widgets/value_input.h>

namespace utils
{
	namespace ui
	{
		class text_input : public value_input<std::string>
		{
		public:
			using base = value_input<std::string>;
			
			text_input(node* parent = nullptr) : base(parent) {
				m_edit_value.reserve(256);
			}

			text_input(
				node* parent = nullptr
				, const std::string& label = {}
				, const std::string& default_value = {}
				, const on_update_t& on_update = nullptr
			) : base(parent, label, default_value, on_update) {}
		};
		//using text_input = value_input<std::string>;
		using text_input_ptr = std::shared_ptr<text_input>;
	}
}