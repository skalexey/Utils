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
			
			text(node* parent, const std::string& text)
				: widget(parent)
				, m_text(text)
			{}

			void set_text(const std::string& text) {
				m_text = text;
			}

			const std::string& get_text() const {
				return m_text;
			}

			virtual void on_show() override = 0;

		private:
			std::string m_text;
		};
		using text_ptr = std::shared_ptr<text>;
	}
}