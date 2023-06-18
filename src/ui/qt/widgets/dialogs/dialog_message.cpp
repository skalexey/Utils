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

			int dialog_message::post_construct()
			{
				auto retcode = base::post_construct();
				if (retcode != 0)
					return retcode;
				ok_button().set_on_before_show([this]() {
					// TODO: align
					// utils::ui::qt::AlignForWidth(width);
				});
				return 0;
			}

			void qt::dialog_message::button_ok_show()
			{
				base::button_ok_show();
			}
		}
	}
}