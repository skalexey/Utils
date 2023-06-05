#pragma once

#include <utils/ui/qt/widget_factory.h>
#include <utils/ui/qt/widgets/text.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			REGISTER_WIDGET(text);

			void qt::text::on_show()
			{
				// qt::TextWrapped("%s", get_text().c_str());
			}
		}
	}
}