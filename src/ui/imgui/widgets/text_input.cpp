#include <SDL.h>
#include <utils/ui/widgets/text_input.h>
#include <utils/io_utils.h>
#include <utils/ui/imgui/widgets/text_input.h>
#include <utils/ui/imgui/sdl_app.h>
#include <utils/ui/imgui/widgets/text_input.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
            bool imgui::text_input::show_input()
			{
				// TODO: do smth with this resize:
				m_edit_value.resize(strlen(m_edit_value.c_str()));
				auto ret = ImGui::InputText(m_input_label.c_str(), m_edit_value.data(), m_edit_value.capacity());
				// TODO: move it to imgui::widget class:
				// Store size and position of ImGui widget for further getting it with get_size() and get_position().
				auto r_min = ImGui::GetItemRectMin();
				auto r_max = ImGui::GetItemRectMax();
				// Save text_input::set_size and set_position for actual resizing and repositioning.
				base::set_size(vec2i(r_max.x - r_min.x, r_max.y - r_min.y));
				base::set_position(vec2i(r_min.x, r_min.y));
				auto& io = ImGui::GetIO();
				if (io.WantTextInput)
				{
					auto& p = get_position();
					auto& s = get_size();
					SDL_Rect rec = { p.x, p.y, s.x, s.y };
					SDL_SetTextInputRect(&rec);
					sdl_app::request_keyboard();
				}
				return ret;
			}
			void imgui::text_input::show_text()
			{
				ImGui::Text("%s", m_label.c_str());
				ImGui::SameLine();
			}
		}
	}
}