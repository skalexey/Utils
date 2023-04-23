#pragma once

#include <memory>
#include <utils/ui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class widget : public virtual ui::widget
			{
				public:
					vec2i get_screen_size() const override;
			};

			using widget_ptr = std::shared_ptr<imgui::widget>;
		}
	}
}