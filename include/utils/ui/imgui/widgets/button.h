#pragma once

#include <memory>
#include <utils/ui/imgui/fwd.h>
#include <utils/ui/widgets/button.h>
#include <utils/ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class button : public imgui::widget, public ui::button
			{
			public:
				using base = ui::button;
				
				button(ui::node* parent = nullptr) 
					: ui::node(parent)
					, base(parent)
					, imgui::widget(parent)
				{}

				button(	ui::node* parent
						, const std::string& label
						, const on_click_t& on_click = nullptr
				)
					: ui::node(parent)
					, base(parent, label, on_click)
					, imgui::widget(parent)
				{}

				void on_show() override;

				const vec2i& text_size();

				const vec2i& get_size() {
					return m_calculated_size;
				}

			private:
				vec2i m_calculated_size;
				WIDGET_REGISTRATOR(imgui::widget_factory, imgui::button);

			};
			using button_ptr = std::shared_ptr<button>;
		}
	}
}