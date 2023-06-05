#pragma once

#include <optional>
#include <utils/ui/widgets/dialogs/dialog_with_buttons.h>
#include <utils/ui/imgui/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			// Order of inheritance is important here as imgui::dialog sets the widget factory first
			class dialog_with_buttons: public imgui::dialog, public ui::dialog_with_buttons
			{
			public:
				using base = ui::dialog_with_buttons;
				
				dialog_with_buttons(ui::node* parent = nullptr)
					: ui::node(parent)
					, base(parent)
					, imgui::dialog(parent)
				{}

				dialog_with_buttons(
					ui::node* parent
					, const std::string& msg
					, const actions_t& actions = {}
					, const std::optional<std::string>& title = {}
				)
					: ui::node(parent)
					, base(parent, msg, actions, title)
					, imgui::dialog(parent)
				{}

				void on_show() override {
					imgui::dialog::on_show();
				}

			private:
				WIDGET_REGISTRATOR(imgui::widget_factory, dialog_with_buttons);
			};
		}
	}
}