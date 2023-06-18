#pragma once

#include <memory>
#include <string>
#include <utils/ui/imgui/fwd.h>
#include <utils/ui/widgets/dialogs/dialog_input_text.h>
#include <utils/ui/imgui/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			// Order of inheritance is important here as imgui::dialog sets the widget factory first
			class dialog_input_text : public imgui::dialog, public ui::dialog_input_text
			{
			public:
				using base = ui::dialog_input_text;
				
				dialog_input_text(ui::node* parent = nullptr)
					: ui::node(parent)
					, base(parent)
					, imgui::dialog(parent)
				{}

				void on_show() override {
					imgui::dialog::on_show();
				}

			private:
				WIDGET_REGISTRATOR(imgui::widget_factory, imgui::dialog_input_text);
			};
		}
	}
}