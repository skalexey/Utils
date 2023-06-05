#pragma once

#include <memory>
#include <utils/ui/imgui/node.h>
#include <utils/ui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class widget : public virtual imgui::node, public virtual ui::widget
			{
				public:
					widget(ui::node* parent = nullptr)
						: ui::node(parent)
						, ui::widget(parent)
						, imgui::node(parent)
					{}

					vec2i get_screen_size() const override;
			};

			using widget_ptr = std::shared_ptr<imgui::widget>;
		}
	}
}