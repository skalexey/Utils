#pragma once

#include <utils/ui/qt/widget_factory.h>
#include <utils/ui/qt/widgets/label.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			REGISTER_WIDGET(label);

			void qt::label::on_show()
			{
				// qt::Text("%s", get_text().c_str());
			}
		}
	}
}