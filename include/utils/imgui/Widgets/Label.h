#pragma once

#include <string>
#include "imgui.h"

namespace ImGui
{
	class LabelWidget
	{
	public:
		LabelWidget() = default;
		LabelWidget(const std::string& label)
			: m_text(label) 
		{}

		bool Show() {
			ImGui::Text(m_text.data());
			return true;
		}

		void SetText(const std::string& text) {
			m_text = text;
		}

		const std::string& GetText() const {
			return m_text;
		}

	private:
		std::string m_text;
	};
}