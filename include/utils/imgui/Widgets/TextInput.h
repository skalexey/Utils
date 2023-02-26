#pragma once

#include <string>
#include "ValueInput.h"

namespace ImGui
{
	class TextInputWidget : public AbstractValueInputWidget<std::string>
	{
	public:
		TextInputWidget(const std::string& label
			, const std::string& default_value
			, const on_update_t& on_update = nullptr
		)
			: AbstractValueInputWidget(label, default_value, on_update)
		{
			m_edit_value.reserve(256);
		}

	protected:
		bool display() override {
			m_edit_value.resize(strlen(m_edit_value.c_str()));
			return ImGui::InputText(m_input_label.c_str(), m_edit_value.data(), m_edit_value.capacity());
		}
	};
}