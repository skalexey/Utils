#pragma once

#include <optional>
#include <utils/ui/widgets/dialogs/dialog_yes_no.h>
#include <utils/ui/qt/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class dialog_yes_no : public qt::dialog, public ui::dialog_yes_no
			{
			public:
				using base = ui::dialog_yes_no;
				
				// Order of initialization is important here as qt::dialog sets the widget factory first
				dialog_yes_no(ui::node* parent = nullptr);

				dialog_yes_no(
					ui::node* parent
					, const std::string& msg
					, const on_answer_t& on_answer = {}
					, const char* yes_text = nullptr
					, const char* no_text = nullptr
					, const std::optional<std::string>& title = {}
				);

				void on_show() override {
					qt::dialog::on_show();
				}

			protected:
				void button_yes_show() override;
			
			private:
				void shared_constructor();
				WIDGET_REGISTRATOR(qt::widget_factory, qt::dialog_yes_no);
			};
		}
	}
}