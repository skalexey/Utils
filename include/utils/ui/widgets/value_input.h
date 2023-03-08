#pragma once

#include <string>
#include <functional>
#include <utils/string_utils.h>
#include <utils/ui/widget.h>

namespace utils
{
	namespace ui
	{
		template <typename T>
		class value_input : public widget
		{
			using base = widget;

		public:
			using on_update_t = std::function<void(const T&)>;
			value_input(
				const std::string& label = {}
				, const T& default_value = {}
				, const on_update_t& on_update = nullptr
			)
				: base()
				, m_edit_value(default_value)
				, m_on_update(on_update)
			{
				if (!label.empty())
					set_label(label);
			}

			void set_on_update(const on_update_t& on_update) { m_on_update = on_update; }
			void on_update() const {
				if (m_on_update)
					m_on_update(m_edit_value);
			}
			void on_update(const T& value) {
				if (m_on_update)
					m_on_update(m_edit_value = value);
			}
			void on_update(const T& value) const {
				if (m_on_update)
					m_on_update(value);
			}
			const on_update_t& get_on_update() const { return m_on_update; }

			void on_show() override {
				show_text();
				if (show_input())
					on_update();
			}

			void set_label(const std::string& label) {
				m_label = label;
				m_input_label = utils::format_str("##%s", m_label.c_str());
			}

			const T& get_value() const { return m_edit_value; }
			void set_value(const T& value) { m_edit_value = value; }

		protected:
			virtual bool show_input() = 0;
			virtual void show_text() = 0;

		protected:
			on_update_t m_on_update;
			T m_edit_value;
			std::string m_label;
			std::string m_input_label;
		};
	}
}