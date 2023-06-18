#pragma once

#include <memory>
#include <string>
#include <utils/ui/widget.h>

namespace utils
{
	namespace ui
	{
		class text : public virtual widget
		{
		public:
			text(node* parent = nullptr) : widget(parent) {};
			
			virtual void set_text(const std::string& text) = 0;
			virtual const std::string& get_text() const = 0;
			virtual void on_show() override = 0;

		protected:
			virtual void on_set_text() {}
		};
		using text_ptr = std::shared_ptr<text>;
	}
}