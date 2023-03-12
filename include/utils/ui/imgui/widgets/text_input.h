#pragma once

#include <string>
#include <utils/ui/widgets/text_input.h>
#include <utils/ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class text_input : public imgui::widget, public utils::ui::text_input
			{
				using base = utils::ui::text_input;
				
			public:
				text_input(const std::string& label = {}
					, const std::string& default_value = {}
					, const on_update_t& on_update = nullptr
				)
					: base(label, default_value, on_update)
				{
					m_edit_value.reserve(256);
				}

			protected:
				bool show_input() override {
					// TODO: do smth with this
					m_edit_value.resize(strlen(m_edit_value.c_str()));
					return ImGui::InputText(m_input_label.c_str(), m_edit_value.data(), m_edit_value.capacity());
				}
				void show_text() override {
					ImGui::Text("%s", m_label.c_str());
					ImGui::SameLine();
				}
			};
		}
	}
}