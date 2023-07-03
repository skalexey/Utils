#pragma once

#include <memory>
#include <utils/ui/window.h>
#include <utils/ui/qt/widget.h>
namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class window : public virtual qt::widget, public virtual ui::window
			{
				using base = ui::window;

			public:
				void on_show() override {
					if (!is_open())
						return;
					// Show the Qt window
						
					base::on_show();
					return;
				}
			};

			using window_ptr = std::shared_ptr<window>;
		}
	}
}