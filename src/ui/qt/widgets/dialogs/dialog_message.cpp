#include <utils/ui/qt/widget_factory.h>
#include <utils/ui/qt/widgets/dialogs/dialog_message.h>
#include <utils/ui/qt/common.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			REGISTER_WIDGET(qt::dialog_message);

			void dialog_message::post_constructed()
			{
				ok_button().set_on_before_show([this]() {
					// TODO: align
					// utils::ui::qt::AlignForWidth(width);
				});
			}

			void qt::dialog_message::button_ok_show()
			{
				base::button_ok_show();
			}

			qt::dialog_message::dialog_message(ui::node* parent)
				: ui::node(parent)
				, base(parent)
				, qt::dialog(parent)
			{
			}

			// In place of a constructor as we only support default one
			void qt::dialog_message::init(
			    const std::string& msg
				, const base::on_answer_t& on_answer
				, const char* ok_text
				, const std::string& title
			)
			{
				base::init(msg, on_answer, ok_text, title);
			}
		}
	}
}