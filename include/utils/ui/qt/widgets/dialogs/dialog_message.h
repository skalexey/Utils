#pragma once

#include <string>
#include <utils/ui/widgets/dialogs/dialog_message.h>
#include <utils/ui/qt/widgets/dialog.h>

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
				
				dialog_message(ui::node* parent = nullptr);

				// In place of a constructor as we only support default one
				void init(
				    const std::string& msg
					, const base::on_answer_t& on_answer = {}
					, const char* ok_text = nullptr
					, const std::string& title = {}
				) override;
				
				void post_constructed();

			protected:
				void on_show() override {
					qt::dialog::on_show();
				}

				void button_ok_show() override;

			private:
				void shared_constructor();
				WIDGET_REGISTRATOR(qt::widget_factory, dialog_message);
			};
		}
	}
}