#pragma once

#include <string>
#include <functional>
#include <utils/string_utils.h>
#include "imgui.h"

namespace ImGui
{
	template <typename T>
	class AbstractValueInputWidget
	{
	public:
		using on_update_t = std::function<void(const T&)>;
		AbstractValueInputWidget(
			const std::string& label
			, const T& default_value
			, const on_update_t& on_update = nullptr
		)
			: m_edit_value(default_value)
			, m_label(label)
			, m_input_label(utils::format_str("##%s", m_label.c_str()))
			, m_on_update(on_update)
		{}

		void SetOnUpdate(const on_update_t& on_update) { m_on_update = on_update; }
		void OnUpdate() const {
			if (m_on_update)
				m_on_update(m_edit_value);
		}
		void OnUpdate(const T& value) {
			if (m_on_update)
				m_on_update(m_edit_value = value);
		}
		void OnUpdate(const T& value) const {
			if (m_on_update)
				m_on_update(value);
		}
		const on_update_t& GetOnUpdate() const { return m_on_update; }

		bool Show() {
			ImGui::Text(m_label.c_str());
			ImGui::SameLine();
			if (display())
			{
				OnUpdate();
				return true;
			}
			return false;
		}

		const T& Value() const { return m_edit_value; }
		void SetValue(const T& value) { m_edit_value = value; }

	protected:
		virtual bool display() = 0;

	protected:
		on_update_t m_on_update;
		T m_edit_value;
		std::string m_label;
		std::string m_input_label;
	};
}