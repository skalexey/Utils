#pragma once

#include <string>
#include <utils/ui/widgets/dialogs/dialog_message.h>
#include <utils/ui/imgui/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			// Order of inheritance is important here as imgui::dialog sets the widget factory first
			class dialog_message : public imgui::dialog, public ui::dialog_message
			{
			public:
				using base = utils::ui::dialog_message;
				
				dialog_message(ui::node* parent = nullptr);

				dialog_message(
					ui::node* parent
					, const std::string& msg
					, const base::on_answer_t& on_answer = {}
					, const char* ok_text = nullptr
					, const std::string& title = {}
				);

			protected:
				void on_show() override {
					imgui::dialog::on_show();
				}

				void button_ok_show() override;

			private:
				void init();
				WIDGET_REGISTRATOR(imgui::widget_factory, dialog_message);
			};
		}
	}
}