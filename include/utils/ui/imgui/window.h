#pragma once

#include <memory>
#include <utils/ui/window.h>
#include <utils/ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class window : public virtual imgui::widget, public virtual ui::window
			{
				using base = ui::window;

			public:
				window(ui::node* parent = nullptr)
					: ui::node(parent)
					, base(parent)
					, imgui::widget(parent)
				{}
				void on_show() override;
			};

			using window_ptr = std::shared_ptr<window>;
		}
	}
}