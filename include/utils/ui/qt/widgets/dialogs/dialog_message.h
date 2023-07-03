#pragma once

#include <string>
#include <utils/ui/widgets/dialogs/dialog_message.h>
#include <utils/ui/qt/widgets/dialog.h>
#include <utils/common_macros.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			// Order of inheritance is important here as qt::dialog sets the widget factory first
			class dialog_message : public qt::dialog, public ui::dialog_message
			{
			public:
				using base = utils::ui::dialog_message;
				
			protected:
				void on_show() override {
					qt::dialog::on_show();
				}

				void button_ok_show() override;

			private:
				int init() override {
					RETURN_IF_NE_0(qt::dialog::init());
					return ui::dialog_message::init();
				}

			private:
				WIDGET_REGISTRATOR(qt::widget_factory, dialog_message);
			};
		}
	}
}