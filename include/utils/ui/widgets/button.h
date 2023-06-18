#pragma once

#include <string>
#include <memory>
#include <functional>
#include <utils/ui/widget.h>
#include <utils/common.h>

namespace utils
{
	namespace ui
	{
		class button : public virtual widget
		{
		public:
			using on_click_t = utils::void_bool_cb;
			using base = widget;

			button(node* parent = nullptr): base(parent) {}

			void init(const std::string& label, const on_click_t& on_click = nullptr)
			{
				m_label = label;
				m_on_click = on_click;
			}

			void set_on_click(const on_click_t& on_click) {
				m_on_click = on_click;
				on_set_on_click(on_click);
			}

			virtual void on_set_on_click(const on_click_t& on_click) {}

			void on_click(bool up) const {
				if (m_on_click)
					m_on_click(up);
			}

			virtual void on_show() override = 0;

			virtual void on_set_text(const std::string& text) {}

			void set_text(const std::string& text) {
				m_label = text;
				on_set_text(m_label);
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