#pragma once

#include <utils/ui/widget.h>
#include <utils/ui/qt/widget.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			vec2i qt::widget::get_screen_size() const
			{
				// TODO: implement
				return vec2i(1920, 1080);
			}
		}
	}
}