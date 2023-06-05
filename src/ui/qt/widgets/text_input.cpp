#include <utils/ui/widgets/text_input.h>
#include <utils/io_utils.h>
#include <utils/ui/qt/widgets/text_input.h>
#include <utils/ui/qt/app.h>
#include <utils/ui/qt/widget_factory.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			REGISTER_WIDGET(text_input);

            bool qt::text_input::show_input()
			{
				// auto ret = qt::InputText(m_input_label.c_str(), m_edit_value.data(), m_edit_value.capacity());
				// TODO: move it to qt::widget class:
				// Store size and position of qt widget for further getting it with get_size() and get_position().
				return true;
			}
			void qt::text_input::show_text()
			{
				// qt::Text("%s", m_label.c_str());
				// qt::SameLine();
			}
		}
	}
}