#pragma once

#include <optional>
#include <utils/ui/widgets/dialogs/dialog_with_buttons.h>
#include <utils/ui/qt/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			// Order of inheritance is important here as qt::dialog sets the widget factory first
			class dialog_with_buttons: public qt::dialog, public ui::dialog_with_buttons
			{
			public:
				using base = ui::dialog_with_buttons;
				
				dialog_with_buttons(ui::node* parent = nullptr)
					: ui::node(parent)
					, base(parent)
					, qt::dialog(parent)
				{}

				// In place of a constructor as we only support default one
				void init(
				    const std::string& msg
					, const actions_t& actions = {}
					, const std::optional<std::string>& title = {}
				) override
				{
					base::init(msg, actions, title);
				}

				void on_show() override {
					utils::ui::qt::dialog::on_show();
				}

			private:
				WIDGET_REGISTRATOR(qt::widget_factory, dialog_with_buttons);
			};
		}
	}
}