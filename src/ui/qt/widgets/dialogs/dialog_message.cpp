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

			void qt::dialog_message::button_ok_show()
			{
				base::button_ok_show();
			}
		}
	}
}