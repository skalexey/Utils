#pragma once

#include <string>
#include <functional>
#include <utils/string_utils.h>
#include "imgui.h"

namespace ImGui
{
	class ButtonWidget
	{
	public:
		using on_click_t = std::function<void()>;
		ButtonWidget(const std::string& label, const on_click_t& on_click = nullptr)
			: m_label(label) 
			, m_on_click(on_click)
		{}

		void SetOnClick(const on_click_t& on_click) {
			m_on_click = on_click;
		}
		void OnClick() const {
			if (m_on_click)
				m_on_click();
		}

		bool Show() {
			if (ImGui::Button(m_label.c_str()))
			{
				OnClick();
				return true;
			}
			return false;
		}

	private:
		on_click_t m_on_click;
		std::string m_label;
	};
}