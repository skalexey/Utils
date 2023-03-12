#pragma once

#include <string>
#include <memory>
#include <functional>
#include <utils/ui/widget.h>

namespace utils
{
	namespace ui
	{
		class button : public virtual widget
		{
		public:
			using on_click_t = std::function<void(bool)>;
			using base = widget;

			button() = default;
			button(const std::string& label, const on_click_t& on_click = nullptr)
				: base()
				, m_label(label) 
				, m_on_click(on_click)
			{}

			void set_on_click(const on_click_t& on_click) {
				m_on_click = on_click;
			}
			void on_click(bool up) const {
				if (m_on_click)
					m_on_click(up);
			}

			virtual void on_show() override = 0;

			void set_text(const std::string& text) {
				m_label = text;
			}

			const std::string& get_text() const {
				return m_label;
			}

		private:
			on_click_t m_on_click;
			std::string m_label;
		};
		using button_ptr = std::shared_ptr<button>;
	}
}