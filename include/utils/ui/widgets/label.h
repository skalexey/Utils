#pragma once

#include <memory>
#include <string>
#include <utils/ui/widget.h>

namespace utils
{
	namespace ui
	{
		class label : public widget
		{
		public:
			label() = default;
			label(const std::string& label)
				: widget()
				, m_text(label) 
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
		using label_ptr = std::shared_ptr<label>;
	}
}