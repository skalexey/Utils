#pragma once

#include <utils/Log.h>
#include <utils/ui/qt/widgets/button_model.h>
// #include "moc_button_model.cpp"

LOG_PREFIX("[qt::button_model]: ");
LOG_POSTFIX("\n");

namespace utils
{
	namespace ui
	{
		namespace qt
        {
            void button_model::buttonClicked() {
				LOG("buttonClicked");
			}
			void button_model::buttonClicked(QString &in) {
				LOG("buttonClicked(" << in.toStdString() << "");
			}
		}
	}
}
