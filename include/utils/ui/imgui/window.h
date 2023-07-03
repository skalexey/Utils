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

			protected:
				virtual bool on_imgui_window_update(float dt) {
					return true;
				}

			//private:
				bool on_update(float dt) override; // Not final for virtual inheritance
			};

			using window_ptr = std::shared_ptr<window>;
		}
	}
}