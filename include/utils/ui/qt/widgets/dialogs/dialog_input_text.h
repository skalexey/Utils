#pragma once

#include <memory>
#include <string>
#include <utils/ui/qt/fwd.h>
#include <utils/ui/widgets/dialogs/dialog_input_text.h>
#include <utils/ui/qt/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			// Order of inheritance is important here as qt::dialog sets the widget factory first
			class dialog_input_text : public qt::dialog, public ui::dialog_input_text
			{
			public:
				using base = ui::dialog_input_text;
				
				dialog_input_text(ui::node* parent = nullptr)
					: ui::node(parent)
					, base(parent)
					, qt::dialog(parent)
				{}

				void on_show() override {
					qt::dialog::on_show();
				}

			private:
				WIDGET_REGISTRATOR(qt::widget_factory, qt::dialog_input_text);
			};
		}
	}
}